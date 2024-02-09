
#ifndef __LOCK_H
#define __LOCK_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

namespace GameJayo { namespace Server {

class Locker
{
private:
    mutable pthread_mutex_t _mutex;
public:
    Locker()
    {
        pthread_mutex_init(&_mutex, NULL);
    }

    ~Locker()
    {
        pthread_mutex_destroy(&_mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }
};

class AutoLock
{
public:
    AutoLock(Locker* pLocker)
    {
        m_pLocker = pLocker;
        if (m_pLocker)m_pLocker->lock();
    }

    ~AutoLock()
    {
        if (m_pLocker)m_pLocker->unlock();
    }
    
private:
    Locker* m_pLocker;
};
}}

#endif //__LOCK_H

