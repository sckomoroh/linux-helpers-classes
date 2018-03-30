#ifndef TESTTASK_MESSAGESERVER_H
#define TESTTASK_MESSAGESERVER_H

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "../thread/Thread.h"
#include "../network/udp/UdpServerSocket.h"
#include "../data/Message.h"

namespace clients
{
    class MessageServerThread : public thread::Thread
    {
    private:
        network::udp::UdpServerSocket m_serverSocket;

    public:
        MessageServerThread()
            : Thread(true)
            , m_serverSocket()
        {
        }

        bool initialize(const char* ipAddress, uint16_t port)
        {
            m_serverSocket.createTargetSocketAddress(ipAddress, port);
            return m_serverSocket.createSocket();
        }

    protected:
        virtual void *threadMethod()
        {
            auto messageSize = sizeof(data::Message);
            char buffer[2] = {0};
            ssize_t receivedBytes;

            auto index = 0u;
            while (!cancelRequested())
            {
                m_serverSocket.waitReadyForRead();
                if (!m_serverSocket.receiveBuffer(buffer, 2, &receivedBytes))
                {
                    printf("Receive message request failed.\n\tCode: %d\n\tError description: %s\n", errno, strerror(errno));
                    continue;
                }

                if (receivedBytes == 2 && buffer[0] == '#' && buffer[1] == '#')
                {
                    auto message = createMessage(index++);

                    m_serverSocket.waitReadyForWrite();
                    if (!m_serverSocket.sendBuffer(&message, messageSize))
                    {
                        printf("Receive message failed.\n\tCode: %d\n\tError description: %s\n", errno, strerror(errno));
                        continue;
                    }
                }

                if (index > 100000)
                {
                    index = 0;
                }
            }

            m_serverSocket.close();

            return nullptr;
        }

    private:
        data::Message createMessage(int index)
        {
            data::Message message;
            message.MessageSize = sizeof(data::Message);
            message.MessageData = index % 11;
            message.MessageType = index % 15;
            message.MessageId = index++;

            return message;
        }
    };
}

#endif //TESTTASK_MESSAGESERVER_H
