#include "AttributeUtility.hpp"
#include <smile/Text.hpp>
#include <assert.h>

namespace
{

const smile::Text AUDIT_NONE            (SMILE_WSTR("*NONE     "));
const smile::Text AUDIT_USER_PROFILE    (SMILE_WSTR("*USRPRF   "));
const smile::Text AUDIT_CHANGE          (SMILE_WSTR("*CHANGE   "));
const smile::Text AUDIT_ALL             (SMILE_WSTR("*ALL      "));
const smile::Text AUDIT_NOT_AVAILABLE   (SMILE_WSTR("*NOTAVL   "));

}

namespace smile
{

AuditMode AttributeUtility::getAuditMode(const uint8_t* const rawAttribute, int ccsid)
{
    Text attribute(rawAttribute, 10, ccsid);
    AuditMode mode = AuditMode_NotAvailable;
    if (attribute == AUDIT_NONE)
        mode = AuditMode_None;
    else if (attribute == AUDIT_USER_PROFILE)
        mode = AuditMode_UserProfile;
    else if (attribute == AUDIT_CHANGE)
        mode = AuditMode_Change;
    else if (attribute == AUDIT_ALL)
        mode = AuditMode_All;
    else
        assert(attribute == AUDIT_NOT_AVAILABLE);
    return mode;
}

}

