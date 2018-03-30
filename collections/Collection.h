#ifndef TESTTASK_COLLECTION_H
#define TESTTASK_COLLECTION_H

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "../thread/Locker.h"
#include "../thread/AutoLocker.h"

namespace collections
{
    template<class TItem>
    class Collection
    {
    private:
        TItem **m_container;
        int64_t m_containerLength;
        int64_t m_lastIndex;
        thread::Locker m_collectionLocker;

    public:
        Collection()
            : m_containerLength(1)
            , m_lastIndex(0)
            , m_collectionLocker()
        {
            auto containerSize = sizeof(TItem *) * m_containerLength;

            m_container = (TItem **) malloc(containerSize);
            memset(m_container, 0, containerSize);
        }

        virtual ~Collection()
        {
            free(m_container);
        }

        void add(TItem *item)
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            addItemToContainer(item);
        }

        void removeByIndex(uint64_t index)
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            removeItemFromContainer(index);
        }

        void remove(TItem* item)
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            auto itemIndex = indexOf(item);

            if (itemIndex == -1)
            {
                return;
            }

            removeItemFromContainer(itemIndex);
        }

        int64_t indexOf(TItem* item) const
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            for (auto i=0u; i<m_lastIndex; i++)
            {
                if (item == m_container[i])
                {
                    return i;
                }
            }

            return -1;
        }

        TItem* firstItem() const
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            if (m_lastIndex < 1)
            {
                return nullptr;
            }

            return m_container[0];
        }

        TItem* lastItem() const
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            if (m_lastIndex < 1)
            {
                return nullptr;
            }

            return m_container[m_lastIndex - 1];
        }

        int64_t count() const
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            return m_lastIndex;
        }

        virtual bool isEmpty() const
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            return m_lastIndex == 0;
        }

        TItem *getByIndex(uint64_t index) const
        {
            thread::AutoLocker autoLock(m_collectionLocker);

            if (index >= m_lastIndex)
            {
                return NULL;
            }

            return m_container[index];
        }

    private:
        void addItemToContainer(TItem *item)
        {
            if (m_lastIndex + 2 >= m_containerLength)
            {
                reallocateContainer();
            }

            m_container[m_lastIndex++] = item;
        }

        void removeItemFromContainer(int64_t index)
        {
            if (index >= m_lastIndex)
            {
                return;
            }

            m_lastIndex--;

            auto containerSize = m_containerLength * sizeof(TItem*);
            auto newContainer = (TItem**)malloc(containerSize);
            memset(newContainer, 0, containerSize);

            auto copyBefore = index;
            auto copyAfter = m_lastIndex - index;

            if (copyBefore > 0)
            {
                memcpy(newContainer, m_container, sizeof(TItem*) * copyBefore);
            }

            if (copyAfter > 0)
            {
                memcpy(&(newContainer[index]), &(m_container[index + 1]), sizeof(TItem*) * copyAfter);
            }

            free(m_container);
            m_container = newContainer;
        }

        void reallocateContainer()
        {
            auto newContainerLength = m_containerLength + 10;
            auto newContainerSize = sizeof(TItem *) * newContainerLength;
            auto newContainer = (TItem **) malloc(newContainerSize);

            memset(newContainer, 0, newContainerSize);
            memcpy(newContainer, m_container, sizeof(TItem*) * m_containerLength);

            free(m_container);

            m_containerLength = newContainerLength;

            m_container = newContainer;
        }
    };
}

#endif //TESTTASK_COLLECTION_H
