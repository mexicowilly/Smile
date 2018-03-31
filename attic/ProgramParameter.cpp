#include <smile/ProgramParameter.hpp>
#include <smile/TextConverter.hpp>
#include <smile/Exception.hpp>
#include "ByteOrder.hpp"
#include "LogUtil.hpp"
#include "TypeInfo.hpp"
#include <assert.h>

std::ostream& operator<< (std::ostream& stream,
                          const smile::ProgramParameter& param)
{
    std::map<std::string, std::string> attrs = param.getAttrMap();
    stream << "[ PARAM ";
    for (std::map<std::string, std::string>::iterator itor = attrs.begin();
         itor != attrs.end();
         itor++)
    {
        stream << itor->first << "=" << itor->second << "; ";
    }
    stream << "]";
    return stream;
}

namespace smile
{

ProgramParameter::ProgramParameter(PassBy passBy)
    : m_passBy(passBy), m_ccsid(TextConverter::getDefaultCCSID())
{
}

ProgramParameter::ProgramParameter(const ProgramParameter& param)
    : m_passBy(param.m_passBy), m_ccsid(param.m_ccsid)
{
}

ProgramParameter& ProgramParameter::operator= (const ProgramParameter& param)
{
    m_passBy = param.m_passBy;
    m_ccsid = param.m_ccsid;
    return *this;
}

std::map<std::string, std::string> ProgramParameter::getAttrMap() const
{
    std::map<std::string, std::string> attrs;
    std::ostringstream stream;
    stream << m_ccsid;
    attrs["CCSID"] = stream.str();
    attrs["pass by"] = (m_passBy == PassBy_Value) ? "value" : "reference";
    std::ostringstream stream2;
    stream2 << getSize();
    attrs["size"] = stream2.str();
    attrs["type"] = TypeInfo::getName(typeid(*this));
    return attrs;
}

void ProgramParameter::setCCSID(int ccsid)
{
    m_ccsid = ccsid;
}

InParameter::InParameter(const std::vector<uint8_t>& inData, PassBy passBy)
    : ProgramParameter(passBy), m_inData(inData)
{
}

InParameter::InParameter(PassBy passBy)
    : ProgramParameter(passBy)
{
}

InParameter::InParameter(const InParameter& param)
    : ProgramParameter(param), m_inData(param.m_inData)
{
}

InParameter& InParameter::operator= (const InParameter& param)
{
    if (&param != this)
    {
        ProgramParameter::operator=(param);
        m_inData = param.m_inData;
    }
    return *this;
}

ProgramParameter* InParameter::duplicate() const
{
    return new InParameter(*this);
}

std::map<std::string, std::string> InParameter::getAttrMap() const
{
    std::map<std::string, std::string> attrs = ProgramParameter::getAttrMap();
    attrs["usage"] = "in";
    attrs["in data"] = "{ " + LogUtil::bytes(const_cast<InParameter*>(this)->getInData()) + "}";
    return attrs;
}

const std::vector<uint8_t>& InParameter::getInData()
{
    return m_inData;
}

uint32_t InParameter::getSize() const
{
    return const_cast<InParameter*>(this)->getInData().size();
}

uint16_t InParameter::getType(uint16_t dataStreamLevel) const
{
    return 11;
}

OutParameter::OutParameter(size_t outSize, PassBy passBy)
    : ProgramParameter(passBy), m_outSize(outSize)
{
}

OutParameter::OutParameter(PassBy passBy)
    : ProgramParameter(passBy), m_outSize(0)
{
}

OutParameter::OutParameter(const OutParameter& param)
    : ProgramParameter(param),
      m_outData(param.m_outData),
      m_outSize(param.m_outSize)
{
}

OutParameter& OutParameter::operator= (const OutParameter& param)
{
    if (&param != this)
    {
        ProgramParameter::operator=(param);
        m_outData = param.m_outData;
        m_outSize = param.m_outSize;
    }
    return *this;
}

ProgramParameter* OutParameter::duplicate() const
{
    return new OutParameter(*this);
}

std::map<std::string, std::string> OutParameter::getAttrMap() const
{
    std::map<std::string, std::string> attrs = ProgramParameter::getAttrMap();
    attrs["usage"] = "out";
    std::ostringstream stream;
    stream << m_outSize;
    attrs["out size"] = stream.str();
    attrs["out data"] = "{ " + LogUtil::bytes(m_outData) + "}";
    return attrs;
}

uint32_t OutParameter::getSize() const
{
    return m_outSize;
}

uint16_t OutParameter::getType(uint16_t dataStreamLevel) const
{
    return 12;
}

InOutParameter::InOutParameter(const std::vector<uint8_t>& inData,
                               size_t outSize,
                               PassBy passBy)
    : ProgramParameter(passBy),
      InParameter(inData, passBy),
      OutParameter(outSize)
{
}

InOutParameter::InOutParameter(PassBy passBy)
    : ProgramParameter(passBy),
      InParameter(passBy),
      OutParameter(passBy)
{
}

InOutParameter::InOutParameter(const InOutParameter& param)
    : ProgramParameter(param),
      InParameter(param),
      OutParameter(param)
{
}

InOutParameter& InOutParameter::operator= (const InOutParameter& param)
{
    InParameter::operator=(param);
    OutParameter::operator=(param);
    return *this;
}

ProgramParameter* InOutParameter::duplicate() const
{
    return new InOutParameter(*this);
}

std::map<std::string, std::string> InOutParameter::getAttrMap() const
{
    std::map<std::string, std::string> inAttrs = InParameter::getAttrMap();
    std::map<std::string, std::string> outAttrs = OutParameter::getAttrMap();
    inAttrs.insert(outAttrs.begin(), outAttrs.end());
    inAttrs["usage"] = "in-out";
    return inAttrs;
}

uint32_t InOutParameter::getSize() const
{
    return std::max(OutParameter::getSize(), InParameter::getSize());
}

uint16_t InOutParameter::getType(uint16_t dataStreamLevel) const
{
    return 13;
}

NullParameter::NullParameter()
    : ProgramParameter(PassBy_Value)
{
}

NullParameter::NullParameter(const NullParameter& param)
    : ProgramParameter(param)
{
}

NullParameter& NullParameter::operator= (const NullParameter& param)
{
    ProgramParameter::operator=(param);
    return *this;
}

ProgramParameter* NullParameter::duplicate() const
{
    return new NullParameter(*this);
}

std::map<std::string, std::string> NullParameter::getAttrMap() const
{
    std::map<std::string, std::string> attrs = ProgramParameter::getAttrMap();
    attrs["usage"] = "null";
    return attrs;
}

uint32_t NullParameter::getSize() const
{
    return 0;
}

uint16_t NullParameter::getType(uint16_t dataStreamLevel) const
{
    return dataStreamLevel < 6 ? 1 : 0xff;
}

Int32InParameter::Int32InParameter(uint32_t value, PassBy passBy)
    : ProgramParameter(passBy),
      InParameter(passBy)
{
    uint32_t networkOrder = H32_TO_N(value);
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&networkOrder);
    setInData(std::vector<uint8_t>(bytes, bytes + sizeof(networkOrder)));
}

Int32InParameter::Int32InParameter(const Int32InParameter& param)
    : ProgramParameter(param),
      InParameter(param)
{
}

Int32InParameter& Int32InParameter::operator= (const Int32InParameter& param)
{
    InParameter::operator=(param);
    return *this;
}

ProgramParameter* Int32InParameter::duplicate() const
{
    return new Int32InParameter(*this);
}

std::map<std::string, std::string> Int32InParameter::getAttrMap() const
{
    std::map<std::string, std::string> attrs = InParameter::getAttrMap();
    const std::vector<uint8_t>& inData = const_cast<Int32InParameter*>(this)->getInData();
    assert(inData.size() == 4);
    uint32_t value;
    memcpy(&value, &inData[0], sizeof(value));
    value = N32_TO_H(value);
    std::ostringstream stream;
    stream << value;
    attrs["value"] = stream.str();
    return attrs;
}

TextInParameter::TextInParameter(const Text& text, PassBy passBy)
    : ProgramParameter(passBy),
      InParameter(passBy),
      m_text(text)
{
}

TextInParameter::TextInParameter(const TextInParameter& param)
    : ProgramParameter(param),
      InParameter(param),
      m_text(param.m_text)
{
}

TextInParameter& TextInParameter::operator= (const TextInParameter& param)
{
    if (&param != this)
    {
        InParameter::operator=(param);
        m_text = param.m_text;
    }
    return *this;
}

ProgramParameter* TextInParameter::duplicate() const
{
    return new TextInParameter(*this);
}

std::map<std::string, std::string> TextInParameter::getAttrMap() const
{
    std::map<std::string, std::string> attrs = InParameter::getAttrMap();
    try
    {
        attrs["text"] = "\"" + m_text.convertToDefault() + "\"";
    }
    catch (IcuException& icue)
    {
    }
    return attrs;
}

const std::vector<uint8_t>& TextInParameter::getInData()
{
    if (InParameter::getInData().empty())
    {
        try
        {
            setInData(m_text.convert(getCCSID()));
        }
        catch (IcuException& e)
        {
            SMILE_LOG_ERROR(LogUtil::smileLogger(),
                "Could not convert text to CCSID " << getCCSID());
        }
    }
    return InParameter::getInData();
}

void TextInParameter::setCCSID(int ccsid)
{
    InParameter::setCCSID(ccsid);
    setInData(std::vector<uint8_t>());
}

NullTerminatedTextInParameter::NullTerminatedTextInParameter(const Text& text, PassBy passBy)
    : ProgramParameter(passBy),
      TextInParameter(text, passBy)
{
}

NullTerminatedTextInParameter::NullTerminatedTextInParameter(const NullTerminatedTextInParameter& param)
    : ProgramParameter(param),
      TextInParameter(param)
{
}

NullTerminatedTextInParameter& NullTerminatedTextInParameter::operator= (const NullTerminatedTextInParameter& param)
{
    TextInParameter::operator=(param);
    return *this;
}

ProgramParameter* NullTerminatedTextInParameter::duplicate() const
{
    return new NullTerminatedTextInParameter(*this);
}

const std::vector<uint8_t>& NullTerminatedTextInParameter::getInData()
{
    const std::vector<uint8_t>& inData = TextInParameter::getInData();
    if (!inData.empty() && inData.back() != 0)
    {
        std::vector<uint8_t> nullTerminated(inData);
        nullTerminated.push_back(0);
        setInData(nullTerminated);
    }
    return TextInParameter::getInData();
}

QsysObjectNameInParameter::QsysObjectNameInParameter(const QsysObjectName& name, PassBy passBy)
    : ProgramParameter(passBy),
      TextInParameter(name.toApiName())
{
}

QsysObjectNameInParameter::QsysObjectNameInParameter(const QsysObjectNameInParameter& param)
    : ProgramParameter(param),
      TextInParameter(param)
{
}

QsysObjectNameInParameter& QsysObjectNameInParameter::operator= (const QsysObjectNameInParameter& param)
{
    TextInParameter::operator=(param);
    return *this;
}

ProgramParameter* QsysObjectNameInParameter::duplicate() const
{
    return new QsysObjectNameInParameter(*this);
}

Int32InOutParameter::Int32InOutParameter(uint32_t inValue, PassBy passBy)
    : ProgramParameter(passBy),
      InOutParameter(passBy)
{
    uint32_t networkOrder = H32_TO_N(inValue);
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&networkOrder);
    setInData(std::vector<uint8_t>(bytes, bytes + sizeof(networkOrder)));
    setOutSize(sizeof(uint32_t));
}

Int32InOutParameter::Int32InOutParameter(const Int32InOutParameter& param)
    : ProgramParameter(param),
      InOutParameter(param)
{
}

Int32InOutParameter& Int32InOutParameter::operator= (const Int32InOutParameter& param)
{
    InOutParameter::operator=(param);
    return *this;
}

ProgramParameter* Int32InOutParameter::duplicate() const
{
    return new Int32InOutParameter(*this);
}

std::map<std::string, std::string> Int32InOutParameter::getAttrMap() const
{
    std::map<std::string, std::string> attrs = InOutParameter::getAttrMap();
    const std::vector<uint8_t>& inData = const_cast<Int32InOutParameter*>(this)->getInData();
    assert(inData.size() == 4);
    uint32_t value;
    memcpy(&value, &inData[0], sizeof(value));
    value = N32_TO_H(value);
    std::ostringstream stream;
    stream << value;
    attrs["in value"] = stream.str();
    if (!getOutData().empty())
    {
        std::ostringstream stream2;
        stream2 << getOutValue();
        attrs["out value"] = stream2.str();
    }
    return attrs;
}

uint32_t Int32InOutParameter::getOutValue() const
{
    return N32_TO_H(*reinterpret_cast<const uint32_t*>(&getOutData()[0]));
}

Int32OutParameter::Int32OutParameter(PassBy passBy)
    : ProgramParameter(passBy),
      OutParameter(sizeof(uint32_t), passBy)
{
}

Int32OutParameter::Int32OutParameter(const Int32OutParameter& param)
    : ProgramParameter(param),
      OutParameter(param)
{
}

Int32OutParameter& Int32OutParameter::operator= (const Int32OutParameter& param)
{
    OutParameter::operator=(param);
    return *this;
}

ProgramParameter* Int32OutParameter::duplicate() const
{
    return new Int32OutParameter(*this);
}

std::map<std::string, std::string> Int32OutParameter::getAttrMap() const
{
    std::map<std::string, std::string> attrs = OutParameter::getAttrMap();
    if (!getOutData().empty())
    {
        std::ostringstream stream;
        stream << getOutValue();
        attrs["value"] = stream.str();
    }
    return attrs;
}

uint32_t Int32OutParameter::getOutValue() const
{
    return N32_TO_H(*reinterpret_cast<const uint32_t*>(&getOutData()[0]));
}

}
