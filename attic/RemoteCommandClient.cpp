#include "RemoteCommandClient.hpp"
#include "RemoteCommandExchangeAttributesRequest.hpp"
#include "RemoteCommandExchangeAttributesReply.hpp"
#include "RunCommandRequest.hpp"
#include "RunCommandReply.hpp"
#include "CallProgramRequest.hpp"
#include "CallProgramReply.hpp"
#include "LogUtil.hpp"
#include "ByteOrder.hpp"
#include "ServicePool.hpp"
#include <algorithm>
#include <memory>
#include <assert.h>

namespace smile
{

RemoteCommandClient::RemoteCommandClient(AS400& as400)
    : ServiceFactory(as400),
      m_commandService(0)
{
}

RemoteCommandClient::~RemoteCommandClient()
{
    ServicePool::getInstance().checkIn(m_commandService);
}

bool RemoteCommandClient::callProgram(const QsysObjectName& program,
                                      ProgramParameterList& parameters,
                                      OS400Message::MessageCount messageCount)
{
    connect();
    std::vector<uint8_t> programBytes = program.toApiName().convert(m_commandService->getCCSID());
    Logger& log = LogUtil::smileLogger();
    if (log.isDebugEnabled())
    {
        for (unsigned i = 0; i < parameters.getSize(); i++)
        {
            parameters[i].setCCSID(m_commandService->getCCSID());
            SMILE_LOG_DEBUG(log, parameters[i]);
        }
    }
    CallProgramRequest request(programBytes,
                               parameters,
                               m_commandService->getDataStreamLevel(),
                               messageCount);
    CallProgramReply* reply;
    try
    {
        reply = m_commandService->sendAndReceive<CallProgramReply>(request);
    }
    catch (InvalidStateException ise)
    {
        delete m_commandService;
        m_commandService = 0;
        throw;
    }
    std::auto_ptr<CallProgramReply> autoReply(reply);
    uint16_t rc = static_cast<uint16_t>(reply->getReturnCode());
    handleReturnCode(__FILE__, __LINE__, "Call program request", rc);
    reply->setCCSID(m_commandService->getCCSID());
    if (rc == 0)
        reply->imbueOutputParameters(parameters);
    else
        m_messages = reply->getMessages(m_as400.getAS400());
    return rc == 0;
}

bool RemoteCommandClient::callServiceProgramProcedure(const QsysObjectName& serviceProgram,
                                                      const Text& procedure,
                                                      ProgramParameterList& parameters,
                                                      OS400Message::MessageCount messageCount,
                                                      ServiceProgramProcedure::ReturnType returnType,
                                                      int32_t& returnCode,
                                                      int32_t& errorCode,
                                                      bool emptyReturnIsSuccess)
{
    static QsysObjectName program(Text(SMILE_WSTR("QSYS")),
                                  Text(SMILE_WSTR("QZRUCLSP")));

    ProgramParameterList allParameters;
    allParameters.addParameter(QsysObjectNameInParameter(serviceProgram));
    allParameters.addParameter(NullTerminatedTextInParameter(procedure));
    allParameters.addParameter(Int32InParameter(returnType));
    std::vector<uint8_t> parameterPassBys;
    for (unsigned i = 0; i < parameters.getSize(); i++)
    {
        uint32_t value = H32_TO_N(static_cast<uint32_t>(parameters[i].getPassBy()));
        parameterPassBys.insert(parameterPassBys.end(),
                                reinterpret_cast<uint8_t*>(&value),
                                reinterpret_cast<uint8_t*>(&value) + sizeof(value));
    }
    allParameters.addParameter(InParameter(parameterPassBys));
    allParameters.addParameter(Int32InParameter(parameters.getSize()));
    std::vector<uint8_t> errorParam(8, 0);
    allParameters.addParameter(InParameter(errorParam));
    allParameters.addParameter(
        OutParameter((returnType == ServiceProgramProcedure::ReturnType_Void) ? 4 : 8));
    allParameters.addParameterList(parameters);
    if (!callProgram(program, allParameters, messageCount))
        return false;
    if (returnType != ServiceProgramProcedure::ReturnType_Void)
    {
        const std::vector<uint8_t>& outData =
            dynamic_cast<OutParameter&>(allParameters[6]).getOutData();
        if (outData.size() == 0)
        {
            if (emptyReturnIsSuccess)
            {
                returnCode = 0;
                errorCode = 0;
            }
            else
            {
                Logger& log = LogUtil::smileLogger();
                if (log.isDebugEnabled())
                {
                    for (std::vector<OS400Message>::iterator itor = m_messages.begin();
                         itor != m_messages.end();
                         itor++)
                    {
                        SMILE_LOG_DEBUG(log, *itor);
                    }
                }
                return false;
            }
        }
        else
        {
            assert(outData.size() == 8);
            memcpy(&returnCode, &outData[0], 4);
            returnCode = N32_TO_H(returnCode);
            if (returnCode == 0)
            {
                errorCode = 0;
            }
            else
            {
                memcpy(&errorCode, &outData[4], 4);
                errorCode = N32_TO_H(errorCode);
            }
        }
    }
    else
    {
        returnCode = 0;
        errorCode = 0;
    }
    for (unsigned i = 0; i < parameters.getSize(); i++)
    {
        OutParameter* destParam = dynamic_cast<OutParameter*>(&parameters[i]);
        OutParameter* srcParam = dynamic_cast<OutParameter*>(&allParameters[i + 7]);
        assert((destParam == 0 && srcParam == 0) || (destParam != 0 && srcParam != 0));
        if (srcParam != 0 && destParam != 0)
            destParam->setOutData(srcParam->getOutData());
    }
    return true;
}

void RemoteCommandClient::connect()
{
    if (m_commandService == 0)
        m_commandService = dynamic_cast<RemoteCommandService*>(ServicePool::getInstance().checkOut(*this));
}

HostService* RemoteCommandClient::createHostService()
{
    RemoteCommandService* service = new RemoteCommandService(m_as400);
    try
    {
        service->connect();
        RemoteCommandExchangeAttributesRequest request;
        RemoteCommandExchangeAttributesReply* reply;
        reply = service->sendAndReceive<RemoteCommandExchangeAttributesReply>(request);
        std::auto_ptr<RemoteCommandExchangeAttributesReply> autoReply(reply);
        handleReturnCode(__FILE__, __LINE__, "Connecting to server", reply->getReturnCode());
        service->imbueAttributes(*reply);
    }
    catch (...)
    {
        delete service;
        throw;
    }
    return service;
}

uint16_t RemoteCommandClient::getServiceIdentifier()
{
    return RemoteCommandService::IDENTIFIER;
}

void RemoteCommandClient::handleReturnCode(const char* const file,
                                           int lineNumber,
                                           const char* const msg,
                                           uint16_t rc)
{
    static RemoteCommandServiceException::ErrorCode exceptionConditions[] =
    {
        RemoteCommandServiceException::ErrorCode_InvalidExchangeAttributesRequest,
        RemoteCommandServiceException::ErrorCode_InvalidDataStreamLevel,
        RemoteCommandServiceException::ErrorCode_InvalidVersion,
        RemoteCommandServiceException::ErrorCode_InvalidCCSID,
        RemoteCommandServiceException::ErrorCode_UnableToProcessRequest,
        RemoteCommandServiceException::ErrorCode_InvalidLL,
        RemoteCommandServiceException::ErrorCode_InvalidServerId,
        RemoteCommandServiceException::ErrorCode_IncompleteData,
        RemoteCommandServiceException::ErrorCode_HostResourceError,
        RemoteCommandServiceException::ErrorCode_InvalidRequestId,
        RemoteCommandServiceException::ErrorCode_ProcessExitPoint,
        RemoteCommandServiceException::ErrorCode_InvalidRequest,
        RemoteCommandServiceException::ErrorCode_InvalidParameter,
        RemoteCommandServiceException::ErrorCode_MaximumExceeded,
        RemoteCommandServiceException::ErrorCode_CallingUserExitProgram,
        RemoteCommandServiceException::ErrorCode_UserExitProgramDeniedRequest,
        RemoteCommandServiceException::ErrorCode_InvalidCCSID_2,
        RemoteCommandServiceException::ErrorCode_Unknown
     };
     static RemoteCommandServiceException::ErrorCode* exceptionConditionsEnd = exceptionConditions +
         (sizeof(exceptionConditions) / sizeof(exceptionConditions[0]));
 #if !defined(NDEBUG)
     RemoteCommandServiceException::ErrorCode* cur = exceptionConditions;
     RemoteCommandServiceException::ErrorCode* next = exceptionConditions;
     for (next++; next != exceptionConditionsEnd; cur++, next++)
         assert(!(*next < *cur));
 #endif
     RemoteCommandServiceException except(file, lineNumber, msg, rc);
     Logger& log = LogUtil::smileLogger();
     SMILE_LOG_DEBUG(log, "Remote command return: " << except);
     if (std::binary_search(exceptionConditions, exceptionConditionsEnd, except.getErrorCode()))
     {
         delete m_commandService;
         m_commandService = 0;
         throw except;
     }
     if (log.isErrorEnabled() &&
         except.getErrorCode() != RemoteCommandServiceException::ErrorCode_None)
     {
         SMILE_LOG_FORCE(LogLevel_Error, log, "Remote command failure: " << except);
     }
}

bool RemoteCommandClient::runCommand(const Text& command, OS400Message::MessageCount messageCount)
{
    connect();
    int ccsid = (m_commandService->getDataStreamLevel() >= 10) ? 1200 : m_commandService->getCCSID();
    std::vector<uint8_t> commandBytes = command.convert(ccsid);
    RunCommandRequest request(commandBytes, m_commandService->getDataStreamLevel(), messageCount, ccsid);
    RunCommandReply* reply;
    try
    {
        reply = m_commandService->sendAndReceive<RunCommandReply>(request);
    }
    catch (InvalidStateException& ise)
    {
        delete m_commandService;
        m_commandService = 0;
        throw;
    }
    std::auto_ptr<RunCommandReply> autoReply(reply);
    uint16_t rc = static_cast<uint16_t>(reply->getReturnCode());
    handleReturnCode(__FILE__, __LINE__, "Run command request", rc);
    reply->setCCSID(ccsid);
    m_messages = reply->getMessages(m_as400.getAS400());
    return rc == 0;
}

}
