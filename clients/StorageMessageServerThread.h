#ifndef TESTTASK_STORAGEMESSAGESERVER_H
#define TESTTASK_STORAGEMESSAGESERVER_H

#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cstdio>

#include "../thread/Thread.h"
#include "../network/tcp/TcpServerSocket.h"
#include "../data/Message.h"
#include "IMessageStorageListener.h"
#include "../collections/Queue.h"

namespace clients
{
    class StorageMessageServerThread
            : public thread::Thread
            , public IMessageStorageListener
    {
    private:
        network::TcpServerSocker m_serverSocket;
        collections::Queue<data::Message> m_messageQueue;

    public:
        StorageMessageServerThread()
            : Thread(true)
        {
        }

        bool initialize(const char* ipAddress, uint16_t port)
        {
            m_serverSocket.createTargetSocketAddress(ipAddress, port);
            return m_serverSocket.createSocket();
        }

    public:
        virtual void onGeneralMessageReceived(data::Message *message)
        {
        }

        virtual void onClientMessageReceived(data::Message *message)
        {
            m_messageQueue.enqueue(message);
        }

    protected:
        virtual void *threadMethod()
        {
            m_serverSocket.waitReadyForRead();
            if (!m_serverSocket.accept())
            {
                printf("Unable to accept incoming connections.\n\tCode: %d\n\tError description: %s\n", errno, strerror(errno));
                return nullptr;
            }

            auto messageDataSize = sizeof(data::Message);

            while (!cancelRequested())
            {
                if (m_messageQueue.isEmpty())
                {
                    continue;
                }

                auto message = m_messageQueue.dequeue();

                m_serverSocket.waitClientReadyForWrite();
                if(!m_serverSocket.sendBuffer(message, messageDataSize))
                {
                    printf("Send client message from server failed.\n\tCode: %d\n\tError description: %s\n", errno, strerror(errno));
                    continue;
                }
            }

            m_serverSocket.closeClient();
            m_serverSocket.close();

            return nullptr;
        }
    };
}

#endif //TESTTASK_STORAGEMESSAGESERVER_H
