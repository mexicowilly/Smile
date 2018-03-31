#include <smile/Notifier.hpp>
#include "Locker.hpp"

namespace
{

inline intptr_t keyType(const void* observer)
{
    return reinterpret_cast<intptr_t>(const_cast<void*>(observer));
}

}

namespace smile
{

Notifier::Notifier()
{
    pthread_mutex_init(&m_mutex, 0);
}

Notifier::~Notifier()
{
    std::for_each(m_records.begin(), m_records.end(), DeleteRecord());
    pthread_mutex_destroy(&m_mutex);
}

Notifier& Notifier::getDefault()
{
    static Notifier notifier;
    return notifier;
}

void Notifier::post(const Notification& notification)
{
    Locker locker(m_mutex);
    for (std::vector<Record*>::iterator itor = m_records.begin();
         itor != m_records.end();
         itor++)
    {
        bool hasName = (*itor)->getName() != 0;
        bool hasSender = (*itor)->getSender() != 0;
        if ((!hasName && !hasSender) ||
            (hasName && !hasSender &&
                *(*itor)->getName() == notification.getName()) ||
            (!hasName && hasSender &&
                (*itor)->getSender() == notification.getSender()) ||
            (hasName && hasSender &&
                *(*itor)->getName() == notification.getName() &&
                (*itor)->getSender() == notification.getSender()))
        {
            (*itor)->run(notification);
        }

    }
}

Notifier::Record::Record(const void* sender)
    : m_name(0), m_sender(sender)
{
}

Notifier::Record::Record(const Text& name, const void* sender)
    : m_name(new Text(name)), m_sender(sender)
{
}

Notifier::Record::~Record()
{
    delete m_name;
}

}
