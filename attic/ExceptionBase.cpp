#include "smile/Exception.hpp"
#include "TypeInfo.hpp"
#include <netdb.h>
#include <ostream>
#include <sstream>
#include <algorithm>

std::ostream& operator<< (std::ostream& stream, const smile::Exception& except)
{
    stream << smile::TypeInfo::getName(typeid(except)) << " in (" << except.getFileName()
           << ", " << except.getLineNumber() << "): " << except.what();
    return stream;
}

namespace smile
{

Exception::Exception(const char* const fileName, int lineNumber)
    : m_fileName(new char[strlen(fileName) + 1]),
      m_what(0),
      m_lineNumber(lineNumber)
{
    strcpy(m_fileName, fileName);
}

Exception::Exception(const char* const fileName,
                     int lineNumber,
                     const char* const msg)
    : m_fileName(new char[strlen(fileName) + 1]),
      m_what(new char[strlen(msg) + 1]),
      m_lineNumber(lineNumber)
{
    strcpy(m_fileName, fileName);
    strcpy(m_what, msg);
}

Exception::Exception(const Exception& except)
    : m_fileName(new char[strlen(except.m_fileName) + 1]),
      m_what(new char[strlen(except.m_what) + 1]),
      m_lineNumber(except.m_lineNumber)
{
    strcpy(m_fileName, except.m_fileName);
    strcpy(m_what, except.m_what);
}

Exception::~Exception() throw()
{
    delete [] m_what;
    delete [] m_fileName;
}

Exception& Exception::operator= (const Exception& except)
{
    if (&except != this)
    {
        delete [] m_fileName;
        m_fileName = new char[strlen(except.m_fileName) + 1];
        strcpy(m_fileName, except.m_fileName);
        delete [] m_what;
        m_what = new char[strlen(except.m_what) + 1];
        strcpy(m_what, except.m_what);
        m_lineNumber = except.m_lineNumber;
    }
    return *this;
}

Exception* Exception::duplicate() const
{
    return new Exception(*this);
}

const char* Exception::what() const throw()
{
    return m_what;
}

}
