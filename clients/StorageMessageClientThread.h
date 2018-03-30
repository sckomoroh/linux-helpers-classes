#ifndef TESTTASK_STORAGEMESSAGECLIENTTHREAD_H
#define TESTTASK_STORAGEMESSAGECLIENTTHREAD_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "../thread/Thread.h"
#include "../network/tcp/TcpServerSocket.h"
#include "../network/tcp/TcpClientSocket.h"
#include "IMessageStorageListener.h"

namespace clients
{
    class StorageMessageClientThread
            : public thread::Thread
    {
    private:
        network::TcpClientSocket m_clientSocket;

    public:
        StorageMessageClientThread()
            : Thread(true)
        {
        }

        bool initialize(const char *ipAddress, uint16_t port)
        {
            m_clientSocket.createTargetSocketAddress(ipAddress, port);
            return m_clientSocket.createSocket();
        }

    protected:
        virtual void *threadMethod()
        {
            m_clientSocket.connect();

            auto messageDataSize = sizeof(data::Message);
            auto buffer = malloc(messageDataSize);

            while (!cancelRequested())
            {
                memset(buffer, 0, messageDataSize);

                m_clientSocket.waitReadyForRead();
                if (!m_clientSocket.receiveBuffer(buffer, messageDataSize, nullptr))
                {
                    printf("Receive client message from server failed.\n\tCode: %d\n\tError description: %s\n", errno, strerror(errno));
                    continue;
                }

                data::Message message;

                memcpy(&message, buffer, messageDataSize);

                printf("ID = %lu\n", message.MessageId);
            };

            free(buffer);

            m_clientSocket.close();

            return nullptr;
        }
    };
}

#endif //TESTTASK_STORAGEMESSAGECLIENTTHREAD_H
