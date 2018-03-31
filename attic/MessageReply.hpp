#if !defined(SMILE_MESSAGEREPLY_HPP__)
#define SMILE_MESSAGEREPLY_HPP__

#include "ReturnCode16BitReply.hpp"
#include "smile/OS400Message.hpp"

namespace smile
{

class MessageReply : public ReturnCode16BitReply
{
public:
    MessageReply(const AccessPacket& packet);

    virtual std::vector<OS400Message> getMessages(AS400& as400) const;
    void setCCSID(int ccsid);

private:
    int m_ccsid;
};

inline MessageReply::MessageReply(const AccessPacket& packet)
    : ReturnCode16BitReply(packet), m_ccsid(-1)
{
}

inline void MessageReply::setCCSID(int ccsid)
{
    m_ccsid = ccsid;
}

}

#endif
