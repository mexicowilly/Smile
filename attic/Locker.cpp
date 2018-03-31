#include "Locker.hpp"

namespace smile
{

Locker::Locker(pthread_mutex_t& mutex)
    : m_mutex(mutex)
{
    pthread_mutex_lock(&m_mutex);
}

Locker::Locker(const pthread_mutex_t& mutex)
    : m_mutex(const_cast<pthread_mutex_t&>(mutex))
{
    pthread_mutex_lock(&m_mutex);
}

Locker::~Locker()
{
    pthread_mutex_unlock(&m_mutex);
}

}
