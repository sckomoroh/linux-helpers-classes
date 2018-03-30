#ifndef TESTTASK_RECEIVERTHREAD_H
#define TESTTASK_RECEIVERTHREAD_H

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <poll.h>

#include "IMessageReceiverListener.h"

#include "../data/Message.h"
#include "../thread/Thread.h"
#include "../network/udp/UdpClientSocket.h"
#include "../collections/Collection.h"

namespace clients
{
    class MessageReceiverThread : public thread::Thread
    {
    private:
        network::udp::UdpClientSocket m_clientSocket;
        collections::Collection<IMessageReceiverListener> m_listeners;

    public:
        MessageReceiverThread()
            : Thread(true)
        {
        }

        bool initialize(const char* ipAddress, uint16_t port)
        {
            m_clientSocket.createTargetSocketAddress(ipAddress, port);
            return m_clientSocket.createSocket();
        }

        void addListener(IMessageReceiverListener* listener)
        {
            m_listeners.add(listener);
        }

    protected:
        virtual void *threadMethod()
        {
            auto messageSize = sizeof(data::Message);
            auto buffer = malloc(messageSize);

            while (!cancelRequested())
            {
                // Send query message
                m_clientSocket.waitReadyForWrite();
                if (!m_clientSocket.sendBuffer((void*)"##", 2))
                {
                    printf("[%lu] Send message request failed.\n\tCode: %d\n\tError description: %s\n", threadId(), errno, strerror(errno));
                    continue;
                }

                memset(buffer, 0, messageSize);

                m_clientSocket.waitReadyForRead();
                if (!m_clientSocket.receiveBuffer(buffer, messageSize, nullptr))
                {
                    printf("[%lu] Receive message from server failed.\n\tCode: %d\n\tError description: %s\n", threadId(), errno, strerror(errno));
                    continue;
                }

                data::Message* message = new data::Message;
                memcpy(message, buffer, messageSize);

                notifyMessageReceived(message);
            }

            free(buffer);

            m_clientSocket.close();

            return nullptr;
        }

        void notifyMessageReceived(data::Message* message)
        {
            for (auto i=0u; i<m_listeners.count(); i++)
            {
                auto listener = m_listeners.getByIndex(i);
                if (listener != nullptr)
                {
                    listener->onMessageReceived(message);
                }
            }
        }
    };
}

#endif //TESTTASK_RECEIVERTHREAD_H
