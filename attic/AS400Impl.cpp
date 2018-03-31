#include "AS400Impl.hpp"
#include "PortRequest.hpp"
#include "PortReply.hpp"
#include "SignonService.hpp"
#include "SignonExchangeAttributesRequest.hpp"
#include "SignonExchangeAttributesReply.hpp"
#include "SignonEndServerRequest.hpp"
#include "SignonInfoRequest.hpp"
#include "SignonInfoReply.hpp"
#include "Locker.hpp"
#include "SignonTextConverter.hpp"
#include "ByteOrder.hpp"
#include "LogUtil.hpp"
#include "CentralClient.hpp"
#include "TypeInfo.hpp"
#include <assert.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <unicode/uchar.h>
#include <sstream>
#include <algorithm>

namespace
{

const int UNKNOWN_PORT = -1;
const int PORT_MAP_SERVICE = 449;
pthread_rwlock_t systemPortMapGuard = PTHREAD_RWLOCK_INITIALIZER;

}

namespace smile
{

AS400Impl::SystemPortMap AS400Impl::sm_systemPortMap = AS400Impl::SystemPortMap();

AS400Impl::AS400Impl(AS400& as400, const Text& systemName, const Text& userName, const Text& password)
    : m_as400(as400),
      m_signonService(0),
      m_authentication(Authentication_Password),
      m_passwordEncryption(PasswordEncryption_Unset),
      m_systemName(systemName),
      m_userName(userName),
      m_localKey(EVP_CIPHER_key_length(EVP_bf_ecb())),
      m_currentSignonTime(0),
      m_lastSignonTime(0),
      m_signonExpirationTime(0),
      m_ccsid(0)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_implMutex, &attr);
    pthread_mutexattr_destroy(&attr);
    try
    {
        m_userName.toUppercase();
    }
    catch (IcuException& icue)
    {
        SMILE_LOG_WARN(LogUtil::smileLogger(), icue);
    }
    RAND_bytes(&m_localKey[0], m_localKey.size());
    m_authenticationBytes = encryptLocal(password.convert("UTF-16"));
}

AS400Impl::~AS400Impl()
{
    pthread_mutex_destroy(&m_implMutex);
}

std::vector<uint8_t> AS400Impl::addWithCarry(const std::vector<uint8_t>& one,
                                             const std::vector<uint8_t>& two) const
{
    if (one.size() != two.size())
        throw InvalidStateException(__FILE__, __LINE__, "The vectors must be the same size");
    unsigned carryBit = 0;
    std::vector<uint8_t> output;
    for (std::vector<uint8_t>::const_reverse_iterator oneItor = one.rbegin(), twoItor = two.rbegin();
         oneItor != one.rend();
         oneItor++, twoItor++)
    {
        unsigned tmp = *oneItor + *twoItor + carryBit;
        carryBit = tmp >> 8;
        output.push_back(tmp);
    }
    std::reverse(output.begin(), output.end());
    return output;
}

void AS400Impl::connectToSignon()
{
    Locker locker(m_implMutex);
    if (m_signonService == 0)
    {
        try
        {
            m_signonService = new SignonService(*this);
            m_signonService->connect();
            if (m_authentication == Authentication_Password)
            {
                m_clientSeed.resize(8);
                RAND_bytes(&m_clientSeed[0], 8);
                while (m_clientSeed[0] >= 0xe0)
                    RAND_bytes(&m_clientSeed[0], 1);
            }
            else
            {
                m_clientSeed.clear();
            }
            SignonExchangeAttributesRequest request = SignonExchangeAttributesRequest(m_clientSeed);
            SignonExchangeAttributesReply* reply =
                m_signonService->sendAndReceive<SignonExchangeAttributesReply>(request);
            std::auto_ptr<SignonExchangeAttributesReply> autoReply(reply);
            if (reply->getReturnCode() != 0)
            {
                throw HostServiceException(__FILE__, __LINE__,
                                           "Exchange attribute reply", reply->getReturnCode());
            }
            m_version = reply->getOS400Version();
            m_level = reply->getOS400Level();
            m_passwordEncryption = reply->getPasswordEncryption();
            m_serverSeed = reply->getServerSeed();
            m_signonJobNameBytes = reply->getJobName();
            Logger& log = LogUtil::smileLogger();
            SMILE_LOG_DEBUG(log, "OS/400 version " << m_version.toText());
            SMILE_LOG_DEBUG(log, "Server level " << m_level);
            SMILE_LOG_DEBUG(log, "Password encryption " <<
                ((m_passwordEncryption == PasswordEncryption_DES) ? "DES" : "SHA1"));
        }
        catch (Exception& e)
        {
            delete m_signonService;
            m_signonService = 0;
            throw e;
        }
    }
}

void AS400Impl::disconnectFromSignon()
{
    Locker locker(m_implMutex);
    if (m_signonService != 0)
    {
        try
        {
            SignonEndServerRequest request;
            m_signonService->send(request);
        }
        catch (Exception& e)
        {
        }
        delete m_signonService;
        m_signonService = 0;
    }
}

std::vector<uint8_t> AS400Impl::decryptLocal(const std::vector<uint8_t>& bytes) const
{
    assert(m_localKey.size() >= static_cast<std::vector<uint8_t>::size_type>(EVP_CIPHER_key_length(EVP_bf_ecb())));
    EVP_CIPHER_CTX context;
    EVP_CIPHER_CTX_init(&context);
    if (!EVP_DecryptInit_ex(&context, EVP_bf_ecb(), 0, &m_localKey[0], 0))
    {
        EVP_CIPHER_CTX_cleanup(&context);
        throw OpenSslException(__FILE__, __LINE__, "Could not initalize Blowfish cipher");
    }
    std::vector<uint8_t> output(bytes.size() + (EVP_CIPHER_block_size(EVP_bf_ecb()) * 2));
    int outputSize1;
    if (!EVP_DecryptUpdate(&context,
                           &output[0],
                           &outputSize1,
                           &bytes[0],
                           bytes.size()))
    {
        EVP_CIPHER_CTX_cleanup(&context);
        throw OpenSslException(__FILE__, __LINE__, "Could not update Blowfish cipher");
    }
    assert(static_cast<std::vector<uint8_t>::size_type>(outputSize1) <= output.size());
    int outputSize2;
    int rc = EVP_DecryptFinal(&context,
                              &output[outputSize1],
                              &outputSize2);
    assert(static_cast<std::vector<uint8_t>::size_type>(outputSize1 + outputSize2) <=
        output.size());
    EVP_CIPHER_CTX_cleanup(&context);
    if (!rc)
        throw OpenSslException(__FILE__, __LINE__, "Could not finalize Blowfish cipher");
    output.resize(outputSize1 + outputSize2);
    return output;
}

size_t AS400Impl::ebcdicStrlen(const std::vector<uint8_t>& bytes) const
{
    std::vector<uint8_t>::const_iterator found = std::find(bytes.begin(), bytes.end(), 0x40);
    return (found == bytes.end()) ? bytes.size() : found - bytes.begin();
}

std::vector<uint8_t> AS400Impl::encryptDesBlock(const std::vector<uint8_t>& key,
                                                const std::vector<uint8_t>& clear) const
{
    if (key.size() != static_cast<std::vector<uint8_t>::size_type>(EVP_CIPHER_key_length(EVP_des_ecb())))
    {
        throw InvalidStateException(__FILE__, __LINE__,
                                    "The key size must be a valid DES key size (8 bytes)");
    }
    if (clear.size() != static_cast<std::vector<uint8_t>::size_type>(EVP_CIPHER_block_size(EVP_des_ecb())))
    {
        throw InvalidStateException(__FILE__, __LINE__,
                                    "Can only encrypt one DES block (8 bytes)");
    }
    EVP_CIPHER_CTX context;
    EVP_CIPHER_CTX_init(&context);
    if (!EVP_CIPHER_CTX_set_padding(&context, 0))
    {
        EVP_CIPHER_CTX_cleanup(&context);
        throw OpenSslException(__FILE__, __LINE__, "Could not turn off DES padding");
    }
    if (!EVP_EncryptInit_ex(&context, EVP_des_ecb(), 0, &key[0], 0))
    {
        EVP_CIPHER_CTX_cleanup(&context);
        throw OpenSslException(__FILE__, __LINE__, "Could not initalize DES cipher");
    }
    std::vector<uint8_t> output(EVP_CIPHER_block_size(EVP_des_ecb()));
    int outLength1;
    int rc = EVP_EncryptUpdate(&context, &output[0], &outLength1, &clear[0], clear.size());
    EVP_CIPHER_CTX_cleanup(&context);
    if (!rc)
        throw OpenSslException(__FILE__, __LINE__, "Could not finalize DES cipher");
    return output;
}

std::vector<uint8_t> AS400Impl::encryptLocal(const std::vector<uint8_t>& bytes) const
{
    assert(m_localKey.size() >=
        static_cast<std::vector<uint8_t>::size_type>(EVP_CIPHER_key_length(EVP_bf_ecb())));
    EVP_CIPHER_CTX context;
    EVP_CIPHER_CTX_init(&context);
    if (!EVP_EncryptInit_ex(&context, EVP_bf_ecb(), 0, &m_localKey[0], 0))
    {
        EVP_CIPHER_CTX_cleanup(&context);
        throw OpenSslException(__FILE__, __LINE__, "Could not initialize Blowfish cipher");
    }
    std::vector<uint8_t> output(bytes.size() + (EVP_CIPHER_block_size(EVP_bf_ecb()) * 2) - 1);
    int outputSize1;
    if (!EVP_EncryptUpdate(&context,
                           &output[0],
                           &outputSize1,
                           &bytes[0],
                           bytes.size()))
    {
        EVP_CIPHER_CTX_cleanup(&context);
        throw OpenSslException(__FILE__, __LINE__, "Could not update Blowfish cipher");
    }
    assert(static_cast<std::vector<uint8_t>::size_type>(outputSize1) <= output.size());
    int outputSize2;
    int rc = EVP_EncryptFinal_ex(&context,
                                 &output[outputSize1],
                                 &outputSize2);
    EVP_CIPHER_CTX_cleanup(&context);
    if (!rc)
        throw OpenSslException(__FILE__, __LINE__, "Could not finalize Blowfish cipher");
    assert(static_cast<std::vector<uint8_t>::size_type>(outputSize1 + outputSize2) <=
        output.size());
    output.resize(outputSize1 + outputSize2);
    return output;
}

std::vector<uint8_t> AS400Impl::generateDesSubstitute(const std::vector<uint8_t>& token,
                                                      const std::vector<uint8_t>& serverSeed,
                                                      const std::vector<uint8_t>& clientSeed,
                                                      const std::vector<uint8_t>& userId,
                                                      const std::vector<uint8_t>& sequence) const
{
    std::vector<uint8_t> rdrseq = addWithCarry(sequence, serverSeed);
    std::vector<uint8_t> nextEncrypted = encryptDesBlock(token, rdrseq);
    std::vector<uint8_t> nextData;
    assert(clientSeed.size() >= 8);
    assert(nextEncrypted.size() >= 8);
    for (unsigned i = 0; i < 8; i++)
        nextData.push_back(nextEncrypted[i] ^ clientSeed[i]);
    nextEncrypted = encryptDesBlock(token, nextData);
    nextData.clear();
    assert(userId.size() >= 8);
    assert(rdrseq.size() >= 8);
    for (unsigned i = 0; i < 8; i++)
        nextData.push_back(userId[i] ^ rdrseq[i]);
    assert(nextEncrypted.size() >= 8);
    for (unsigned i = 0; i < 8; i++)
        nextData[i] ^= nextEncrypted[i];
    nextEncrypted = encryptDesBlock(token, nextData);
    assert(userId.size() >= 10);
    nextData.clear();
    nextData.push_back(userId[8]);
    nextData.push_back(userId[9]);
    nextData.resize(8, 0x40);
    for (unsigned i = 0; i < 8; i++)
    {
        nextData[i] ^= rdrseq[i];
        nextData[i] ^= nextEncrypted[i];
    }
    nextEncrypted = encryptDesBlock(token, nextData);
    assert(sequence.size() >= 8);
    assert(nextEncrypted.size() >= 8);
    for (unsigned i = 0; i < 8; i++)
        nextData[i] = sequence[i] ^ nextEncrypted[i];
    return encryptDesBlock(token, nextData);
}

std::vector<uint8_t> AS400Impl::generateDesToken(const std::vector<uint8_t>& userId,
                                                 const std::vector<uint8_t>& password) const
{
    assert(userId.size() <= 10);
    assert(password.size() <= 16);
    std::vector<uint8_t> userIdLocal(userId);
    userIdLocal.resize(10, 0x40);
    if (ebcdicStrlen(userIdLocal) > 8)
    {
        userIdLocal[0] ^= userIdLocal[8] & 0xc0;
        userIdLocal[1] ^= (userIdLocal[8] & 0x30) << 2;
        userIdLocal[2] ^= (userIdLocal[8] & 0x0c) << 4;
        userIdLocal[3] ^= (userIdLocal[8] & 0x03) << 6;
        userIdLocal[4] ^= userIdLocal[9] & 0xc0;
        userIdLocal[5] ^= (userIdLocal[9] & 0x30) << 2;
        userIdLocal[6] ^= (userIdLocal[9] & 0x0c) << 4;
        userIdLocal[7] ^= (userIdLocal[9] & 0x03) << 6;
    }
    userIdLocal.resize(8);
#if defined(SMILE_PASSWORD_TRACE)
    Logger& log = LogUtil::smileLogger();
    SMILE_LOG_DEBUG(log, "Folded user id: " << LogUtil::bytes(userIdLocal));
#endif
    std::vector<uint8_t> pwd1;
    std::vector<uint8_t> pwd2;
    if (ebcdicStrlen(password) <= 8)
    {
        pwd1.assign(password.begin(), password.end());
        pwd1.resize(8, 0x40);
    }
    else
    {
        pwd1.assign(password.begin(), password.begin() + 8);
        pwd2.assign(password.begin() + 8, password.end());
    }
    pwd2.resize(8, 0x40);

    for (std::vector<uint8_t>::iterator itor = pwd1.begin();
         itor != pwd1.end();
         itor++)
    {
        *itor ^= 0x55;
    }
    pwd1[0] = (pwd1[0] << 1) | (pwd1[1] >> 7);
    pwd1[1] = (pwd1[1] << 1) | (pwd1[2] >> 7);
    pwd1[2] = (pwd1[2] << 1) | (pwd1[3] >> 7);
    pwd1[3] = (pwd1[3] << 1) | (pwd1[4] >> 7);
    pwd1[4] = (pwd1[4] << 1) | (pwd1[5] >> 7);
    pwd1[5] = (pwd1[5] << 1) | (pwd1[6] >> 7);
    pwd1[6] = (pwd1[6] << 1) | (pwd1[7] >> 7);
    pwd1[7] <<= 1;
#if defined(SMILE_PASSWORD_TRACE)
    SMILE_LOG_DEBUG(log, "Key: " << LogUtil::bytes(pwd));
    SMILE_LOG_DEBUG(log, "Clear: " << LogUtil::bytes(userIdLocal));
#endif
    std::vector<uint8_t> token = encryptDesBlock(pwd1, userIdLocal);
    if (ebcdicStrlen(password) > 8)
    {
        for (std::vector<uint8_t>::iterator itor = pwd2.begin();
             itor != pwd2.end();
             itor++)
        {
            *itor ^= 0x55;
        }
        pwd2[0] = (pwd2[0] << 1) | (pwd2[1] >> 7);
        pwd2[1] = (pwd2[1] << 1) | (pwd2[2] >> 7);
        pwd2[2] = (pwd2[2] << 1) | (pwd2[3] >> 7);
        pwd2[3] = (pwd2[3] << 1) | (pwd2[4] >> 7);
        pwd2[4] = (pwd2[4] << 1) | (pwd2[5] >> 7);
        pwd2[5] = (pwd2[5] << 1) | (pwd2[6] >> 7);
        pwd2[6] = (pwd2[6] << 1) | (pwd2[7] >> 7);
        pwd2[7] <<= 1;
        pwd2 = encryptDesBlock(pwd2, userIdLocal);
        assert(token.size() == 8);
        assert(pwd2.size() == 8);
        for (int i = 0; i < 8; i++)
        {
            token[i] ^= pwd2[i];
        }
    }
#if defined(SMILE_PASSWORD_TRACE)
    SMILE_LOG_DEBUG(log, "Token: " << LogUtil::bytes(token));
#endif
    return token;
}

std::vector<uint8_t> AS400Impl::generateSha1Substitute(const std::vector<uint8_t>& token,
                                                       const std::vector<uint8_t>& serverSeed,
                                                       const std::vector<uint8_t>& clientSeed,
                                                       const std::vector<uint8_t>& userId,
                                                       const std::vector<uint8_t>& sequence) const
{
    EVP_MD_CTX context;
    if (!EVP_DigestInit(&context, EVP_sha1()))
        throw OpenSslException(__FILE__, __LINE__, "Could not initialize SHA-1 digest");
    if (!EVP_DigestUpdate(&context, &token[0], token.size()) ||
        !EVP_DigestUpdate(&context, &serverSeed[0], serverSeed.size()) ||
        !EVP_DigestUpdate(&context, &clientSeed[0], clientSeed.size()) ||
        !EVP_DigestUpdate(&context, &userId[0], userId.size()) ||
        !EVP_DigestUpdate(&context, &sequence[0], sequence.size()))
    {
        EVP_MD_CTX_cleanup(&context);
        throw OpenSslException(__FILE__, __LINE__, "Could not update SHA-1 digest");
    }
    std::vector<uint8_t> messageDigest(EVP_MAX_MD_SIZE);
    unsigned returnedLength;
    int rc = EVP_DigestFinal(&context, &messageDigest[0], &returnedLength);
    EVP_MD_CTX_cleanup(&context);
    if (!rc)
        throw OpenSslException(__FILE__, __LINE__, "Could not finalize SHA-1 digest");
    messageDigest.resize(returnedLength);
    return messageDigest;
}

std::vector<uint8_t> AS400Impl::generateSha1Token(const std::vector<uint8_t>& userId,
                                                  const std::vector<uint8_t>& password) const
{
    EVP_MD_CTX context;
    if (!EVP_DigestInit(&context, EVP_sha1()))
        throw OpenSslException(__FILE__, __LINE__, "Could not initialize SHA-1 digest");
    if (!EVP_DigestUpdate(&context, &userId[0], userId.size()) ||
        !EVP_DigestUpdate(&context, &password[0], password.size()))
    {
        EVP_MD_CTX_cleanup(&context);
        throw OpenSslException(__FILE__, __LINE__, "Could not update SHA-1 digest");
    }
    std::vector<uint8_t> messageDigest(EVP_MAX_MD_SIZE);
    unsigned returnedLength;
    int rc = EVP_DigestFinal(&context, &messageDigest[0], &returnedLength);
    EVP_MD_CTX_cleanup(&context);
    if (!rc)
        throw OpenSslException(__FILE__, __LINE__, "Could not finalize SHA-1 digest");
    messageDigest.resize(returnedLength);
    return messageDigest;
}

int AS400Impl::getCCSID()
{
    if (m_ccsid == 0)
    {
        CentralClient central(getAS400());
        m_ccsid = central.getCCSID();
    }
    return m_ccsid;
}

std::vector<uint8_t> AS400Impl::getEncryptedPassword(const std::vector<uint8_t>& clientSeed,
                                                     const std::vector<uint8_t>& serverSeed) const
{
    assert(m_authentication == Authentication_Password);
#if defined(SMILE_PASSWORD_TRACE)
    Logger& log = LogUtil::smileLogger();
    log.debug("Client seed: %s", LogUtil::bytes(clientSeed).c_str());
    log.debug("Server seed: %s", LogUtil::bytes(serverSeed).c_str());
#endif
    if (m_authenticationBytes.empty())
        throw InvalidStateException(__FILE__, __LINE__, "Password is not set");
    std::vector<uint8_t> output;
    uint8_t rawSequence[] = { 0, 0, 0, 0, 0, 0, 0, 1 };
    std::vector<uint8_t> sequence(rawSequence, rawSequence + sizeof(rawSequence));
    if (m_passwordEncryption == PasswordEncryption_DES)
    {
        Text password(decryptLocal(m_authenticationBytes), "UTF-16");
        password.trim();
        password.toUppercase();
        std::vector<uint8_t> ebcdicPassword = SignonTextConverter::getInstance().
            convertToSignonEbcdic(password);
        bool allNumbers = true;
        for (std::vector<uint8_t>::iterator itor = ebcdicPassword.begin();
             itor != ebcdicPassword.end();
             itor++)
        {
            if (*itor < 0xf0 || *itor > 0xf9)
            {
                allNumbers = false;
                break;
            }
        }
        if (allNumbers)
            ebcdicPassword.insert(ebcdicPassword.begin(), 0xd8);
        ebcdicPassword.resize(10, 0x40);
#if defined(SMILE_PASSWORD_TRACE)
        log.debug("Password in EBCDIC: %s", LogUtil::bytes(ebcdicPassword).c_str());
#endif
        Text userName(m_userName);
        userName.trim();
        userName.toUppercase();
        std::vector<uint8_t> ebcdicUser = SignonTextConverter::getInstance().
            convertToSignonEbcdic(userName);
        ebcdicUser.resize(10, 0x40);
        std::vector<uint8_t> token = generateDesToken(ebcdicUser, ebcdicPassword);
        output = generateDesSubstitute(token, serverSeed, clientSeed, ebcdicUser, sequence);
    }
    else if (m_passwordEncryption == PasswordEncryption_SHA1)
    {
        std::vector<uint8_t> userIdBytes = SignonTextConverter::getInstance().
            convertToSignonUtf16(m_userName);
        Text password(decryptLocal(m_authenticationBytes), "UTF-16");
        password.trim();
        std::vector<uint8_t> passwordBytes = SignonTextConverter::getInstance().
            convertToSignonUtf16(password);
        std::vector<uint8_t> token = generateSha1Token(userIdBytes, passwordBytes);
        output = generateSha1Substitute(token, serverSeed, clientSeed, userIdBytes, sequence);
    }
    else if (m_passwordEncryption == PasswordEncryption_Unset)
    {
        throw InvalidStateException(__FILE__, __LINE__, "Password encryption type is not set");
    }
    else
    {
        throw InvalidStateException(__FILE__, __LINE__, "Password encryption type is not supported");
    }
    return output;
}


int AS400Impl::getPort(const HostService& service) const
{
    Logger& log = LogUtil::smileLogger();
    Text mapKey = m_systemName + Text(TypeInfo::getName(typeid(*this)).c_str(), "US-ASCII");
    const char* serviceName = service.getName();
    std::string nameConverted = m_systemName.convertToDefault();
    bool hasWriteLock = false;
    pthread_rwlock_rdlock(&systemPortMapGuard);
    SystemPortMap::iterator found = sm_systemPortMap.find(mapKey);
    if (found == sm_systemPortMap.end())
    {
        SMILE_LOG_DEBUG(log, "No port map found for " << mapKey);
        hasWriteLock = true;
        pthread_rwlock_unlock(&systemPortMapGuard);
        pthread_rwlock_wrlock(&systemPortMapGuard);
        std::pair<SystemPortMap::iterator, bool> ins = sm_systemPortMap.insert(
            std::make_pair(mapKey, std::map<uint16_t, int>()));
        assert(ins.second);
        found = ins.first;
    }
    std::map<uint16_t, int>::iterator found2 =
        found->second.find(service.getIdentifier());
    if (found2 == found->second.end())
    {
        SMILE_LOG_DEBUG(log, "Retrieving port for " << nameConverted <<
            ":" << serviceName);
        Connection connection = Connection(m_systemName, PORT_MAP_SERVICE);
        connection.connect();
        PortRequest request = PortRequest(serviceName);
        request.send(connection);
        PortReply reply;
        reply.receive(connection);
        if (reply.getPort() == UNKNOWN_PORT)
        {
            pthread_rwlock_unlock(&systemPortMapGuard);
            throw InvalidStateException(__FILE__, __LINE__, "Unable to map server port");
        }
        if (!hasWriteLock)
        {
            pthread_rwlock_unlock(&systemPortMapGuard);
            pthread_rwlock_wrlock(&systemPortMapGuard);
        }
        found2->second = reply.getPort();
    }
    int port = found2->second;
    pthread_rwlock_unlock(&systemPortMapGuard);
    SMILE_LOG_DEBUG(log, nameConverted << ":" << serviceName << "on port " << port);
    return port;
}

const OS400Version& AS400Impl::getVersion()
{
    prepareConnection();
    return m_version;
}

void AS400Impl::prepareConnection()
{
    if (m_passwordEncryption == PasswordEncryption_Unset)
    {
        connectToSignon();
        disconnectFromSignon();
    }
}

void AS400Impl::signon(bool keepConnection)
{
    Locker locker(m_implMutex);
    if (m_currentSignonTime == 0)
    {
        connectToSignon();
        assert(m_authentication == Authentication_Password);
        std::vector<uint8_t> userId = SignonTextConverter::getInstance().
            convertToSignonEbcdic(m_userName);
        userId.resize(10, 0x40);
        std::vector<uint8_t> encryptedPassword = getEncryptedPassword(m_clientSeed, m_serverSeed);
        SignonInfoRequest infoRequest = SignonInfoRequest(userId,
                                                          encryptedPassword,
                                                          m_authentication,
                                                          m_level);
        SignonInfoReply* infoReply = m_signonService->sendAndReceive<SignonInfoReply>(infoRequest);
        if (infoReply->getReturnCode() != 0)
        {
            throw HostServiceException(__FILE__, __LINE__,
                                       "Could not get signon info", infoReply->getReturnCode());
        }
        m_currentSignonTime = infoReply->getCurrentSignonTime();
        m_lastSignonTime = infoReply->getLastSignonTime();
        m_signonExpirationTime = infoReply->getExpirationTime();
        m_ccsid = infoReply->getCCSID();
        std::vector<uint8_t> returnedUser = infoReply->getUserId();
        if (!returnedUser.empty())
        {
            m_userName = SignonTextConverter::getInstance().
                convertFromSignonEbcdic(infoReply->getUserId());
        }
        m_jobName = Text(&m_signonJobNameBytes[0], m_signonJobNameBytes.size(), m_ccsid);
        Logger& log = LogUtil::smileLogger();
        SMILE_LOG_DEBUG(log, "Server CCSID: " << m_ccsid);
        SMILE_LOG_DEBUG(log, "Job name: " << m_jobName);
        if (!keepConnection)
            disconnectFromSignon();
    }
}

}
