#if !defined(SMILE_PROGRAMPARAMETER_HPP__)
#define SMILE_PROGRAMPARAMETER_HPP__

#include <smile/Duplicable.hpp>
#include <smile/QsysObjectName.hpp>
#include <map>

namespace smile
{

class ProgramParameter;

}

std::ostream& operator<< (std::ostream& stream,
                          const smile::ProgramParameter& param);

namespace smile
{

class ProgramParameter : public Duplicable<ProgramParameter>
{
public:
    enum PassBy
    {
        PassBy_Value        = 1,
        PassBy_Reference    = 2
    };

    ProgramParameter& operator= (const ProgramParameter& param);

    int getCCSID() const;
    PassBy getPassBy() const;
    virtual uint32_t getSize() const = 0;
    virtual uint16_t getType(uint16_t dataStreamLevel) const = 0;
    virtual void setCCSID(int ccsid);
    void setPassBy(const PassBy passBy);

protected:
    friend std::ostream& ::operator<< (std::ostream& stream,
                                       const smile::ProgramParameter& param);

    ProgramParameter(PassBy passBy);
    ProgramParameter(const ProgramParameter& param);
    virtual std::map<std::string, std::string> getAttrMap() const;

private:
    PassBy m_passBy;
    int m_ccsid;
};

class InParameter : public virtual ProgramParameter
{
public:
    InParameter(const std::vector<uint8_t>& inData, PassBy passBy = PassBy_Value);
    InParameter(const InParameter& param);

    InParameter& operator= (const InParameter& param);

    virtual ProgramParameter* duplicate() const;
    virtual const std::vector<uint8_t>& getInData();
    virtual uint32_t getSize() const;
    virtual uint16_t getType(uint16_t dataStreamLevel) const;
    void setInData(const std::vector<uint8_t>& inData);

protected:
    InParameter(PassBy passBy);
    virtual std::map<std::string, std::string> getAttrMap() const;

private:
    std::vector<uint8_t> m_inData;
};

class OutParameter : public virtual ProgramParameter
{
public:
    OutParameter(size_t outSize, PassBy passBy = PassBy_Value);
    OutParameter(const OutParameter& param);

    OutParameter& operator= (const OutParameter& param);

    virtual ProgramParameter* duplicate() const;
    const std::vector<uint8_t>& getOutData() const;
    virtual uint32_t getSize() const;
    virtual uint16_t getType(uint16_t dataStreamLevel) const;
    void setOutData(const std::vector<uint8_t>& outData);
    void setOutSize(size_t outSize);

protected:
    OutParameter(PassBy passBy);
    virtual std::map<std::string, std::string> getAttrMap() const;

private:
    std::vector<uint8_t> m_outData;
    size_t m_outSize;
};

class InOutParameter : public InParameter, public OutParameter
{
public:
    InOutParameter(const std::vector<uint8_t>& inData, size_t outSize, PassBy passBy = PassBy_Value);
    InOutParameter(const InOutParameter& param);

    InOutParameter& operator= (const InOutParameter& param);

    virtual ProgramParameter* duplicate() const;
    virtual uint32_t getSize() const;
    virtual uint16_t getType(uint16_t dataStreamLevel) const;

protected:
    InOutParameter(PassBy passBy);
    virtual std::map<std::string, std::string> getAttrMap() const;
};

class NullParameter : public ProgramParameter
{
public:
    NullParameter();
    NullParameter(const NullParameter& param);

    NullParameter& operator= (const NullParameter& param);

    virtual ProgramParameter* duplicate() const;
    virtual uint32_t getSize() const;
    virtual uint16_t getType(uint16_t dataStreamLevel) const;

protected:
    virtual std::map<std::string, std::string> getAttrMap() const;
};

class Int32InParameter : public InParameter
{
public:
    Int32InParameter(uint32_t value, PassBy passBy = PassBy_Value);
    Int32InParameter(const Int32InParameter& param);

    Int32InParameter& operator= (const Int32InParameter& param);

    virtual ProgramParameter* duplicate() const;

protected:
    virtual std::map<std::string, std::string> getAttrMap() const;
};

class TextInParameter : public InParameter
{
public:
    TextInParameter(const Text& text, PassBy passBy = PassBy_Value);
    TextInParameter(const TextInParameter& param);

    TextInParameter& operator= (const TextInParameter& param);

    virtual ProgramParameter* duplicate() const;
    virtual const std::vector<uint8_t>& getInData();
    virtual void setCCSID(int ccsid);

protected:
    virtual std::map<std::string, std::string> getAttrMap() const;

private:
    Text m_text;
};

class NullTerminatedTextInParameter : public TextInParameter
{
public:
    NullTerminatedTextInParameter(const Text& text, PassBy passBy = PassBy_Value);
    NullTerminatedTextInParameter(const NullTerminatedTextInParameter& param);

    NullTerminatedTextInParameter& operator= (const NullTerminatedTextInParameter& param);

    virtual ProgramParameter* duplicate() const;
    virtual const std::vector<uint8_t>& getInData();
};

class QsysObjectNameInParameter : public TextInParameter
{
public:
    QsysObjectNameInParameter(const QsysObjectName& name, PassBy passBy = PassBy_Value);
    QsysObjectNameInParameter(const QsysObjectNameInParameter& param);

    QsysObjectNameInParameter& operator= (const QsysObjectNameInParameter& param);

    virtual ProgramParameter* duplicate() const;
};

class Int32InOutParameter : public InOutParameter
{
public:
    Int32InOutParameter(uint32_t inValue, PassBy passBy = PassBy_Value);
    Int32InOutParameter(const Int32InOutParameter& param);

    Int32InOutParameter& operator= (const Int32InOutParameter& param);

    virtual ProgramParameter* duplicate() const;
    uint32_t getOutValue() const;

protected:
    virtual std::map<std::string, std::string> getAttrMap() const;
};

class Int32OutParameter : public OutParameter
{
public:
    Int32OutParameter(PassBy passBy = PassBy_Value);
    Int32OutParameter(const Int32OutParameter& param);

    Int32OutParameter& operator= (const Int32OutParameter& param);

    virtual ProgramParameter* duplicate() const;
    uint32_t getOutValue() const;

protected:
    virtual std::map<std::string, std::string> getAttrMap() const;
};

inline int ProgramParameter::getCCSID() const
{
    return m_ccsid;
}

inline ProgramParameter::PassBy ProgramParameter::getPassBy() const
{
    return m_passBy;
}

inline void ProgramParameter::setPassBy(const PassBy passBy)
{
    m_passBy = passBy;
}

inline void InParameter::setInData(const std::vector<uint8_t>& inData)
{
    m_inData = inData;
}

inline const std::vector<uint8_t>& OutParameter::getOutData() const
{
    return m_outData;
}

inline void OutParameter::setOutData(const std::vector<uint8_t>& outData)
{
    m_outData = outData;
}

inline void OutParameter::setOutSize(size_t outSize)
{
    m_outSize = outSize;
}

}

#endif
