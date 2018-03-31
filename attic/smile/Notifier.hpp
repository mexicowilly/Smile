#if !defined(SMILE_NOTIFIER_HPP__)
#define SMILE_NOTIFIER_HPP__

#include <smile/Notification.hpp>
#include <smile/Duplicable.hpp>

namespace smile
{

class Notifier
{
public:
    static Notifier& getDefault();

    Notifier();
    ~Notifier();

    template <class Target>
    void addObserver(Target& observer,
                     void (Target::*action)(const Notification&),
                     const void* sender = 0);
    template <class Target>
    void addObserver(Target& observer,
                     void (Target::*action)(const Notification&),
                     const Text& name,
                     const void* sender = 0);
    void post(const Notification& notification);
    template <class Target>
    void removeObserver(const Target& observer, const void* sender = 0);
    template <class Target>
    void removeObserver(const Target& observer, const Text& name, const void* sender = 0);

private:
    class Record
    {
    public:
        virtual ~Record();

        Text* getName();
        const void* getSender();
        virtual const void* getTarget() = 0;
        virtual void run(const Notification& notification) = 0;

    protected:
        Record(const Text& name, const void* sender);
        Record(const void* sender);

    private:
        Text* m_name;
        const void* m_sender;
    };

    template <class Target>
    class ObjectRecord : public Record
    {
    public:
        typedef void (Target::*Action)(const Notification&);

        ObjectRecord(Target& target,
                     Action action,
                     const Text& name,
                     const void* sender = 0);
        ObjectRecord(Target& target,
                     Action action,
                     const void* sender = 0);

        virtual const void* getTarget();
        virtual void run(const Notification& notification);

    private:
        Target& m_target;
        Action m_action;
    };

    class NotTargetAndSender : public std::unary_function<Record*, bool>
    {
    public:
        NotTargetAndSender(const void* target, const void* sender);
        bool operator() (Record* record);

    private:
        const void* m_target;
        const void* m_sender;
    };

    class DeleteRecord : public std::unary_function<Record*, void>
    {
    public:
        void operator() (Record* record);
    };

    class NotTargetNameAndSender : public std::unary_function<Record*, bool>
    {
    public:
        NotTargetNameAndSender(const void* target, const Text& name, const void* sender);
        bool operator() (Record* record);

    private:
        const void* m_target;
        const Text& m_name;
        const void* m_sender;
    };

    Notifier(const Notifier&);
    Notifier& operator= (const Notifier&);

    pthread_mutex_t m_mutex;
    std::vector<Record*> m_records;
};

template <class Target>
void Notifier::addObserver(Target& observer,
                           void (Target::*action)(const Notification&),
                           const void* sender)
{
    pthread_mutex_lock(&m_mutex);
    m_records.push_back(new ObjectRecord<Target>(observer, action, sender));
    pthread_mutex_unlock(&m_mutex);
}

template <class Target>
void Notifier::addObserver(Target& observer,
                           void (Target::*action)(const Notification&),
                           const Text& name,
                           const void* sender)
{
    pthread_mutex_lock(&m_mutex);
    m_records.push_back(new ObjectRecord<Target>(observer, action, name, sender));
    pthread_mutex_unlock(&m_mutex);
}

template <class Target>
void Notifier::removeObserver(const Target& observer, const void* sender)
{
    pthread_mutex_lock(&m_mutex);
    std::vector<Record*>::iterator end = m_records.end();
    std::vector<Record*>::iterator part = std::stable_partition(m_records.begin(),
                                                                end,
                                                                NotTargetAndSender(&observer, sender));
    std::for_each(part, end, DeleteRecord());
    m_records.erase(part, end);
    pthread_mutex_unlock(&m_mutex);
}

template <class Target>
void Notifier::removeObserver(const Target& observer, const Text& name, const void* sender)
{
    pthread_mutex_lock(&m_mutex);
    std::vector<Record*>::iterator end = m_records.end();
    std::vector<Record*>::iterator part = std::stable_partition(m_records.begin(),
                                                                end,
                                                                NotTargetNameAndSender(&observer, name, sender));
    std::for_each(part, end, DeleteRecord());
    m_records.erase(part, end);
    pthread_mutex_unlock(&m_mutex);
}

inline Text* Notifier::Record::getName()
{
    return m_name;
}

inline const void* Notifier::Record::getSender()
{
    return m_sender;
}

template <class Target>
Notifier::ObjectRecord<Target>::ObjectRecord(Target& target,
                                             Action action,
                                             const Text& name,
                                             const void* sender)
    : Record(name, sender), m_target(target), m_action(action)
{
}

template <class Target>
Notifier::ObjectRecord<Target>::ObjectRecord(Target& target,
                                             Action action,
                                             const void* sender)
    : Record(sender), m_target(target), m_action(action)
{
}

template <class Target>
const void* Notifier::ObjectRecord<Target>::getTarget()
{
    return &m_target;
}

template <class Target>
void Notifier::ObjectRecord<Target>::run(const Notification& notification)
{
    (m_target.*m_action)(notification);
}

inline Notifier::NotTargetAndSender::NotTargetAndSender(const void* target, const void* sender)
    : m_target(target), m_sender(m_sender)
{
}

inline bool Notifier::NotTargetAndSender::operator() (Record* record)
{
    return !(m_target == record->getTarget() &&
             (m_sender == 0 || m_sender != record->getSender()));
}

inline void Notifier::DeleteRecord::operator() (Record* record)
{
    delete record;
}

inline Notifier::NotTargetNameAndSender::NotTargetNameAndSender(const void* target,
                                                                const Text& name,
                                                                const void* sender)
    : m_target(target), m_name(name), m_sender(sender)
{
}

inline bool Notifier::NotTargetNameAndSender::operator() (Record* record)
{
    return !(m_target == record->getTarget() &&
             (record->getName() == 0 || m_name == *record->getName()) &&
             (m_sender == 0 || m_sender == record->getSender()));
}

}

#endif
