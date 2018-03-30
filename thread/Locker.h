#ifndef TESTTASK_LOCKER_H
#define TESTTASK_LOCKER_H

#include <pthread.h>

namespace thread
{
    class Locker
    {
    private:
        pthread_mutex_t m_mutex;

    public:
        Locker()
            : m_mutex(PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP)
        {
        }

        void lock()
        {
            pthread_mutex_lock(&m_mutex);
        }

        void unlock()
        {
            pthread_mutex_unlock(&m_mutex);
        }
    };
}

#endif //TESTTASK_LOCKER_H
