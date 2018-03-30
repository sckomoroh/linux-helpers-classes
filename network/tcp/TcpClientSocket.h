#ifndef TESTTASK_TCPSOCKETCLIENT_H
#define TESTTASK_TCPSOCKETCLIENT_H

#include <zconf.h>
#include "TcpSocket.h"

namespace network
{
    class TcpClientSocket : public TcpSocket
    {
    public:
        bool connect()
        {
            return ::connect(
                    m_socket,
                    (struct sockaddr *)&m_targetSocketAddress,
                    sizeof(m_targetSocketAddress)) != -1;
        }

        virtual bool sendBuffer(void* buffer, size_t bufferSize)
        {
            return send(
                    m_socket,
                    buffer,
                    bufferSize,
                    MSG_WAITALL) > 0;
        }

        virtual bool receiveBuffer(void* buffer, size_t bufferSize, ssize_t* receivedBytes)
        {
            auto received = recv(
                    m_socket,
                    buffer,
                    bufferSize,
                    MSG_WAITALL);

            if (receivedBytes != nullptr)
            {
                *receivedBytes = received;
            }

            return received > 0;
        }
    };
}

#endif //TESTTASK_TCPSOCKETCLIENT_H
