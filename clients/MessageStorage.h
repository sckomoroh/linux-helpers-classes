#ifndef TESTTASK_MESSAGESTORAGE_H
#define TESTTASK_MESSAGESTORAGE_H

#include "../data/Message.h"
#include "../thread/Locker.h"
#include "../thread/AutoLocker.h"
#include "../collections/Collection.h"
#include "IMessageReceiverListener.h"
#include "IMessageStorageListener.h"

namespace clients
{
    class MessageStorage : public IMessageReceiverListener
    {
    private:
        collections::Collection<data::Message> m_messagesCollection;
        collections::Collection<IMessageStorageListener> m_listeners;
        thread::Locker m_verifyLocker;

    public:
        void addListener(IMessageStorageListener* listener)
        {
            m_listeners.add(listener);
        }

        void onMessageReceived(data::Message *message) override
        {
            if (isMessageExists(message))
            {
                return;
            }

            m_messagesCollection.add(message);

            notifyGeneralMessageReceived(message);

            if (message->MessageData == 10)
            {
                notifyClientMessageReceived(message);
            }
        }

    private:
        void notifyGeneralMessageReceived(data::Message *message)
        {
            auto listenersCount = m_listeners.count();
            for (auto i=0ul; i<listenersCount; i++)
            {
                auto listener = m_listeners.getByIndex(i);
                if (listener != nullptr)
                {
                    listener->onGeneralMessageReceived(message);
                }
            }
        }

        void notifyClientMessageReceived(data::Message *message)
        {
            auto listenersCount = m_listeners.count();
            for (auto i=0u; i<listenersCount; i++)
            {
                auto listener = m_listeners.getByIndex(i);
                if (listener != nullptr)
                {
                    listener->onClientMessageReceived(message);
                }
            }
        }

        bool isMessageExists(data::Message *message) const
        {
            thread::AutoLocker locker(m_verifyLocker);

            auto messageCount = m_messagesCollection.count();
            for(auto i=0ul; i<messageCount; i++)
            {
                auto currentMessage = m_messagesCollection.getByIndex(i);
                if (currentMessage->MessageId == message->MessageId)
                {
                    return true;
                }
            }

            return false;
        }
    };
}

#endif //TESTTASK_MESSAGESTORAGE_H
