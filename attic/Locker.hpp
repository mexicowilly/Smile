#if !defined(SMILE_LOCKER_HPP__)
#define SMILE_LOCKER_HPP__

#include <pthread.h>

namespace smile
{

class Locker
{
public:
    Locker(pthread_mutex_t& mutex);
    Locker(const pthread_mutex_t& mutex);
    ~Locker();

private:
    Locker(const Locker&);
    Locker& operator= (const Locker&);

    pthread_mutex_t& m_mutex;
};

}

#endif
