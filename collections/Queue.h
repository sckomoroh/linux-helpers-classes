#ifndef TESTTASK_QUEUE_H
#define TESTTASK_QUEUE_H

#include "Collection.h"

namespace collections
{
    template<class TItem>
    class Queue : private Collection<TItem>
    {
    private:
        thread::Locker m_queueLocker;

    public:
        virtual ~Queue() = default;

        void enqueue(TItem *item)
        {
            thread::AutoLocker autoLock(m_queueLocker);

            Collection<TItem>::add(item);
        }

        TItem* dequeue()
        {
            thread::AutoLocker autoLock(m_queueLocker);

            auto lastItem = Collection<TItem>::lastItem();

            Collection<TItem>::remove(lastItem);

            return lastItem;
        }

        virtual bool isEmpty() const
        {
            thread::AutoLocker autoLock(m_queueLocker);

            return Collection<TItem>::isEmpty();
        }
    };
}

#endif //TESTTASK_QUEUE_H
