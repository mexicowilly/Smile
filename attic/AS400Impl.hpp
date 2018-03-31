#if !defined(SMILE_AS400IMPL_HPP__)
#define SMILE_AS400IMPL_HPP__

#include <inttypes.h>
#include <map>
#include <vector>
#include <smile/AS400.hpp>

namespace smile
{

class HostService;
class SignonService;

class AS400Impl
{
public:
    enum PasswordEncryption
    {
        PasswordEncryption_Unset,
        PasswordEncryption_DES,
        PasswordEncryption_SHA1
    };

    enum Authentication
    {
        Authentication_Password
    };

    AS400Impl(AS400& as400, const Text& systemName, const Text& userName, const Text& password);
    virtual ~AS400Impl();

    AS400& getAS400() const;
    Authentication getAuthentication() const;
    int getCCSID();
    std::vector<uint8_t> getEncryptedPassword(const std::vector<uint8_t>& clientSeed,
                                              const std::vector<uint8_t>& serverSeed) const;
    int getPort(const HostService& service) const;
    const Text& getSystemName() const;
    const Text& getUserName() const;
    const OS400Version& getVersion();
    void prepareConnection();
    void signon(bool keepConnection);

private:
    typedef std::map<Text, std::map<uint16_t, int> > SystemPortMap;

    std::vector<uint8_t> addWithCarry(const std::vector<uint8_t>& one,
                                      const std::vector<uint8_t>& two) const;
    void connectToSignon();
    std::vector<uint8_t> decryptLocal(const std::vector<uint8_t>& bytes) const;
    void disconnectFromSignon();
    size_t ebcdicStrlen(const std::vector<uint8_t>& bytes) const;
    std::vector<uint8_t> encryptDesBlock(const std::vector<uint8_t>& key,
                                         const std::vector<uint8_t>& clear) const;
    std::vector<uint8_t> encryptLocal(const std::vector<uint8_t>& bytes) const;
    std::vector<uint8_t> generateDesSubstitute(const std::vector<uint8_t>& token,
                                               const std::vector<uint8_t>& serverSeed,
                                               const std::vector<uint8_t>& clientSeed,
                                               const std::vector<uint8_t>& userId,
                                               const std::vector<uint8_t>& sequence) const;
    std::vector<uint8_t> generateDesToken(const std::vector<uint8_t>& userId,
                                          const std::vector<uint8_t>& password) const;
    std::vector<uint8_t> generateSha1Substitute(const std::vector<uint8_t>& token,
                                                const std::vector<uint8_t>& serverSeed,
                                                const std::vector<uint8_t>& clientSeed,
                                                const std::vector<uint8_t>& userId,
                                                const std::vector<uint8_t>& sequence) const;
    std::vector<uint8_t> generateSha1Token(const std::vector<uint8_t>& userId,
                                           const std::vector<uint8_t>& password) const;

    static SystemPortMap sm_systemPortMap;

    AS400& m_as400;
    SignonService* m_signonService;
    Authentication m_authentication;
    std::vector<uint8_t> m_clientSeed;
    std::vector<uint8_t> m_serverSeed;
    OS400Version m_version;
    uint16_t m_level;
    PasswordEncryption m_passwordEncryption;
    std::vector<uint8_t> m_signonJobNameBytes;
    pthread_mutex_t m_implMutex;
    Text m_systemName;
    std::vector<uint8_t> m_authenticationBytes;
    Text m_userName;
    std::vector<uint8_t> m_localKey;
    Time m_currentSignonTime;
    Time m_lastSignonTime;
    Time m_signonExpirationTime;
    int m_ccsid;
    Text m_jobName;
};

inline AS400& AS400Impl::getAS400() const
{
    return m_as400;
}

inline AS400Impl::Authentication AS400Impl::getAuthentication() const
{
    return m_authentication;
}

inline const Text& AS400Impl::getSystemName() const
{
    return m_systemName;
}

inline const Text& AS400Impl::getUserName() const
{
    return m_userName;
}

}

#endif
