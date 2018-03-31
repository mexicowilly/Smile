#if !defined(SMILE_MESSAGEFILE_HPP__)
#define SMILE_MESSAGEFILE_HPP__

#include <smile/AS400.hpp>
#include <smile/QsysObjectName.hpp>
#include <smile/OS400Message.hpp>

namespace smile
{

class MessageFile
{
public:
    MessageFile(AS400& as400, const QsysObjectName& fileName);
    MessageFile(const MessageFile& file);

    MessageFile& operator= (const MessageFile& file);

    AS400& getAS400() const;
    const QsysObjectName& getFileName() const;
    OS400Message getMessage(const Text& identifier,
                            const std::vector<uint8_t>& substitutionData =
                                std::vector<uint8_t>()) const;

private:
    AS400* m_as400;
    QsysObjectName m_fileName;
};

inline AS400& MessageFile::getAS400() const
{
    return *m_as400;
}

inline const QsysObjectName& MessageFile::getFileName() const
{
    return m_fileName;
}

}

#endif
