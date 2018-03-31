#if !defined(SMILE_ATTRIBUTEUTILITY_HPP__)
#define SMILE_ATTRIBUTEUTILITY_HPP__

#include <smile/AuditMode.hpp>
#include <inttypes.h>

namespace smile
{

class AttributeUtility
{
public:
    static AuditMode getAuditMode(const uint8_t* const rawAttribute, int ccsid);
};

}

#endif
