#ifndef TESTTASK_THREAD_H
#define TESTTASK_THREAD_H

#include "pthread.h"

namespace thread
{
    void *threadFunctionInvoker(void *param);

    class Thread
    {
    private:
        pthread_t m_thread;
        bool m_cancelRequested;
        bool m_waitInDestructor;

    public:
        Thread(bool waitInDestructor = true)
            : m_thread(0)
            , m_cancelRequested(false)
            , m_waitInDestructor(waitInDestructor)
        {
        }

        virtual ~Thread()
        {
            if (m_waitInDestructor)
            {
                join();
            }
        }

        void *start()
        {
            pthread_create(&m_thread, nullptr, threadFunctionInvoker, this);
        }

        void join()
        {
            if (m_thread == 0)
            {
                return;
            }

            pthread_join(m_thread, nullptr);

            m_thread = 0;
        }

        void cancel()
        {
            m_cancelRequested = true;
        }

        uint64_t threadId()
        {
            return m_thread;
        }

    protected:
        Thread()
            : m_cancelRequested(false)
        {
        }

        virtual void *threadMethod() = 0;

        bool cancelRequested()
        {
            return m_cancelRequested;
        }

    private:
        friend void *threadFunctionInvoker(void *param);
    };

    void* threadFunctionInvoker(void *param)
    {
        auto *threadObject = (Thread *) param;

        return threadObject->threadMethod();
    }
}


#endif //TESTTASK_THREAD_H
