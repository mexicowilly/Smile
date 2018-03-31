#include <smile/MessageFile.hpp>
#include <smile/Program.hpp>
#include <smile/Exception.hpp>
#include "OS400DataStructure.hpp"
#include <assert.h>

namespace smile
{

MessageFile::MessageFile(AS400& as400, const QsysObjectName& fileName)
    : m_as400(&as400), m_fileName(fileName)
{
}

MessageFile::MessageFile(const MessageFile& file)
    : m_as400(file.m_as400), m_fileName(file.m_fileName)
{
}

MessageFile& MessageFile::operator= (const MessageFile& file)
{
    if (&file != this)
    {
        m_as400 = file.m_as400;
        m_fileName = file.m_fileName;
    }
    return *this;
}

OS400Message MessageFile::getMessage(const Text& identifier,
                                     const std::vector<uint8_t>& substitutionData) const
{
    Text no(SMILE_WSTR("*NO       "));
    Text yes(SMILE_WSTR("*YES      "));
    ProgramParameterList parameters;
    parameters.addParameter(OutParameter(8192));
    parameters.addParameter(Int32InParameter(8192));
    parameters.addParameter(TextInParameter(Text(SMILE_WSTR("RTVM0300"))));
    parameters.addParameter(TextInParameter(identifier));
    parameters.addParameter(TextInParameter(m_fileName.toApiName()));
    parameters.addParameter(InParameter(substitutionData));
    parameters.addParameter(Int32InParameter(substitutionData.size()));
    parameters.addParameter(TextInParameter(substitutionData.empty() ? no : yes));
    parameters.addParameter(TextInParameter(no));
    parameters.addParameter(InParameter(std::vector<uint8_t>(8, 0)));
    Program rtvm(*m_as400,
                 QsysObjectName(Text(SMILE_WSTR("QSYS")), Text(SMILE_WSTR("QMHRTVM"))),
                 parameters);
    if (!rtvm.run())
        throw Exception(__FILE__, __LINE__, "Unable to run remote program QMHRTVM");
    OS400Message message(*m_as400);
    message.setMessageFile(m_fileName);
    message.setSubstitutionData(substitutionData);
    OS400DataStructure data(dynamic_cast<const OutParameter&>(rtvm.getParameters()[0]).getOutData());
    assert(data.getInt32(0) <= 8192);
    int ccsid = data.getInt32(48);
    if (ccsid == 0 || ccsid == 65535)
        ccsid = rtvm.getCCSID();
    message.setSeverity(data.getInt32(8));
    message.setMessageIdentifier(data.getText(26, 7, ccsid));
    message.setDefaultReply(data.getText(data.getInt32(52), data.getInt32(56), ccsid));
    message.setText(data.getText(data.getInt32(64), data.getInt32(68), ccsid));
    message.setHelp(data.getText(data.getInt32(76), data.getInt32(80), ccsid));
    return message;
}

}
