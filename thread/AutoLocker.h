#ifndef TESTTASK_AUTOLOCKER_H
#define TESTTASK_AUTOLOCKER_H

#include "Locker.h"

namespace thread
{
    class AutoLocker
    {
    private:
        Locker* m_locker;

    public:
        AutoLocker(Locker& locker)
        {
            m_locker = &locker;
            m_locker->lock();
        }

        AutoLocker(const Locker& locker)
        {
            m_locker = &(Locker&)(locker);
            m_locker->lock();
        }

        virtual ~AutoLocker()
        {
            m_locker->unlock();
        }
    };
}

#endif //TESTTASK_AUTOLOCKER_H
