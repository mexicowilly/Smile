#include <smile/Notification.hpp>

namespace smile
{

Notification::Notification(const Text& name, const void* sender)
    : m_name(name), m_sender(sender)
{
}

Notification::Notification(const Text& name, const void* sender, const Properties& data)
    : m_name(name), m_sender(sender), m_data(data)
{
}

Notification::Notification(const Notification& notification)
    : m_name(notification.m_name), m_sender(notification.m_sender), m_data(notification.m_data)
{
}

Notification& Notification::operator= (const Notification& notification)
{
    if (&notification != this)
    {
        m_name = notification.m_name;
        m_sender = notification.m_sender;
        m_data = notification.m_data;
    }
    return *this;
}

bool Notification::operator== (const Notification& notification) const
{
    return m_name == notification.m_name &&
           m_sender == notification.m_sender &&
           m_data == notification.m_data;

}

}
