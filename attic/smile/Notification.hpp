#if !defined(SMILE_NOTIFICATION_HPP__)
#define SMILE_NOTIFICATION_HPP__

#include <smile/Properties.hpp>

namespace smile
{

class Notification
{
public:
    Notification(const Text& name, const void* sender);
    Notification(const Text& name, const void* sender, const Properties& data);
    Notification(const Notification& notification);

    Notification& operator= (const Notification& notification);
    bool operator== (const Notification& notification) const;
    bool operator!= (const Notification& notification) const;

    const Properties& getData() const;
    const Text& getName() const;
    const void* getSender() const;

private:
    Text m_name;
    const void* m_sender;
    Properties m_data;
};

inline bool Notification::operator!= (const Notification& notification) const
{
    return !operator==(notification);
}

inline const Properties& Notification::getData() const
{
    return m_data;
}

inline const Text& Notification::getName() const
{
    return m_name;
}

inline const void* Notification::getSender() const
{
    return m_sender;
}

}

#endif
