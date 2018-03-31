#if !defined(SMILE_QLGPATHNAME_HPP__)
#define SMILE_QLGPATHNAME_HPP__

#include "smile/ProgramParameter.hpp"
#include "smile/IfsObjectName.hpp"

namespace smile
{

class QlgPathName : public InParameter
{
public:
    QlgPathName(const IfsObjectName& name, PassBy passBy = PassBy_Value);
    QlgPathName(const QlgPathName& path);

    QlgPathName& operator= (const QlgPathName& path);

    virtual ProgramParameter* duplicate() const;
    const IfsObjectName& getObjectName() const;
    virtual const std::vector<uint8_t>& getInData();
    virtual void setCCSID(int ccsid);

protected:
    virtual std::map<std::string,std::string> getAttrMap() const;

private:
    IfsObjectName m_name;
};

inline const IfsObjectName& QlgPathName::getObjectName() const
{
    return m_name;
}

}

#endif
