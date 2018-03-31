#if !defined(SMILE_SERVICEPROGRAMPROCEDURE_HPP__)
#define SMILE_SERVICEPROGRAMPROCEDURE_HPP__

#include <smile/Program.hpp>

namespace smile
{

class ServiceProgramProcedure : public Program
{
public:
    enum ReturnType
    {
        ReturnType_Void     = 0,
        ReturnType_Integer  = 3
    };

    ServiceProgramProcedure(AS400& as400);
    ServiceProgramProcedure(AS400& as400,
                            const QsysObjectName& serviceProgram,
                            const Text& procedure);
    ServiceProgramProcedure(AS400& as400,
                            const QsysObjectName& serviceProgram,
                            const Text& procedure,
                            const ProgramParameterList& parameters);
    ServiceProgramProcedure(const ServiceProgramProcedure& procedure);

    ServiceProgramProcedure& operator= (const ServiceProgramProcedure& procedure);

    bool emptyReturnIsSuccess() const;
    int32_t getErrorCode() const;
    const std::vector<OS400Message>& getMessages() const;
    const Text& getProcedure() const;
    int32_t getReturnCode() const;
    ReturnType getReturnType() const;
    virtual bool run();
    void setProcedure(const Text& procedure);
    void setEmptyReturnIsSuccess(bool newState);
    void setReturnType(ReturnType returnType);

private:
    Text m_procedure;
    int32_t m_errorCode;
    int32_t m_returnCode;
    ReturnType m_returnType;
    bool m_emptyReturnIsSuccess;
};

inline bool ServiceProgramProcedure::emptyReturnIsSuccess() const
{
    return m_emptyReturnIsSuccess;
}

inline int32_t ServiceProgramProcedure::getErrorCode() const
{
    return m_errorCode;
}

inline const Text& ServiceProgramProcedure::getProcedure() const
{
    return m_procedure;
}

inline int32_t ServiceProgramProcedure::getReturnCode() const
{
    return m_returnCode;
}

inline ServiceProgramProcedure::ReturnType ServiceProgramProcedure::getReturnType() const
{
    return m_returnType;
}

inline void ServiceProgramProcedure::setEmptyReturnIsSuccess(bool newState)
{
    m_emptyReturnIsSuccess = newState;
}

inline void ServiceProgramProcedure::setProcedure(const Text& procedure)
{
    m_procedure = procedure;
}

inline void ServiceProgramProcedure::setReturnType(ReturnType returnType)
{
    m_returnType = returnType;
}

}

#endif
