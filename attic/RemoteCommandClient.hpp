#if !defined(SMILE_REMOTECOMMANDCLIENT_HPP__)
#define SMILE_REMOTECOMMANDCLIENT_HPP__

#include <smile/OS400Message.hpp>
#include <smile/ProgramParameter.hpp>
#include <smile/ServiceProgramProcedure.hpp>
#include <smile/QsysObjectName.hpp>
#include "RemoteCommandService.hpp"
#include "ServiceFactory.hpp"

namespace smile
{

class RemoteCommandClient : public ServiceFactory
{
public:
    RemoteCommandClient(AS400& as400);
    ~RemoteCommandClient();

    bool callProgram(const QsysObjectName& program,
                     ProgramParameterList& parameters,
                     OS400Message::MessageCount messageCount);
    bool callServiceProgramProcedure(const QsysObjectName& serviceProgram,
                                     const Text& procedure,
                                     ProgramParameterList& parameters,
                                     OS400Message::MessageCount messageCount,
                                     ServiceProgramProcedure::ReturnType returnType,
                                     int32_t& returnCode,
                                     int32_t& errorCode,
                                     bool emptyReturnIsSuccess);
    virtual HostService* createHostService();
    int getCCSID() const;
    const std::vector<OS400Message>& getMessages() const;
    virtual uint16_t getServiceIdentifier();
    bool runCommand(const Text& command, OS400Message::MessageCount messageCount);

private:
    void connect();
    void handleReturnCode(const char* const file,
                          int lineNumber,
                          const char* const msg,
                          uint16_t rc);

    std::vector<OS400Message> m_messages;
    RemoteCommandService* m_commandService;
};

inline int RemoteCommandClient::getCCSID() const
{
    return m_commandService->getCCSID();
}

inline const std::vector<OS400Message>& RemoteCommandClient::getMessages() const
{
    return m_messages;
}

}

#endif
