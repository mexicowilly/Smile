#ifndef SMILE_REPLY_HPP
#define SMILE_REPLY_HPP

#include <memory>

namespace smile
{

class reply
{
public:
    virtual ~reply() {}
};

template <typename reply_type>
std::unique_ptr<reply_type> cast_reply(std::unique_ptr<reply>&& r)
{
    reply_type* rp = dynamic_cast<reply_type*>(r.get());
    if (rp == nullptr)
        throw std::bad_cast();
    r.release();
    return std::unique_ptr<reply_type>(rp);
}

}

#endif
