#include "QlgPathName.hpp"
#include <smile/TextConverter.hpp>
#include <smile/Exception.hpp>
#include "Packet.hpp"
#include "LogUtil.hpp"
#include <assert.h>

namespace
{

const size_t QLG_PATH_NAME_T_SIZE = 32;

}

namespace smile
{

QlgPathName::QlgPathName(const IfsObjectName& name, PassBy passBy)
    : ProgramParameter(passBy),
      InParameter(passBy),
      m_name(name)
{
}

QlgPathName::QlgPathName(const QlgPathName& path)
    : ProgramParameter(path),
      InParameter(path),
      m_name(path.m_name)
{
}

QlgPathName& QlgPathName::operator= (const QlgPathName& path)
{
    if (&path != this)
    {
        InParameter::operator=(path);
        m_name = path.m_name;
    }
    return *this;
}

ProgramParameter* QlgPathName::duplicate() const
{
    return new QlgPathName(*this);
}

std::map<std::string,std::string> QlgPathName::getAttrMap() const
{
    std::map<std::string, std::string> attrs = InParameter::getAttrMap();
    try
    {
        attrs["path"] = m_name.getAbsolutePath().convertToDefault();
    }
    catch (IcuException& icue)
    {
    }
    return attrs;
}

const std::vector<uint8_t>& QlgPathName::getInData()
{
    if (InParameter::getInData().empty())
    {
        try
        {
            std::vector<uint8_t> pathBytes = m_name.getAbsolutePath().convert(getCCSID());
            smile::Packet packet(QLG_PATH_NAME_T_SIZE + pathBytes.size());
            packet.setInt32(0, getCCSID());
            packet.setInt32(16, pathBytes.size());
            smile::TextConverter slashConverter(SMILE_DEFAULT_CONVERTER, getCCSID());
            std::vector<uint8_t> slashBytes =
                slashConverter.convert(reinterpret_cast<const uint8_t*>("/"));
            assert(slashBytes.size() == 1);
            packet.setInt8(20, slashBytes[0]);
            packet.setBytes(QLG_PATH_NAME_T_SIZE, pathBytes);
            setInData(packet.getBytes(0, packet.getSize()));

        }
        catch (IcuException& e)
        {
            SMILE_LOG_ERROR(LogUtil::smileLogger(),
                "Could not convert text to CCSID " << getCCSID());
        }
    }
    return InParameter::getInData();
}

void QlgPathName::setCCSID(int ccsid)
{
    InParameter::setCCSID(ccsid);
    setInData(std::vector<uint8_t>());
}

}
