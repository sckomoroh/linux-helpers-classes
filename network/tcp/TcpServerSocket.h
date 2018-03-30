#ifndef TESTTASK_TCPSOCKETSERVER_H
#define TESTTASK_TCPSOCKETSERVER_H

#include "TcpSocket.h"

namespace network
{
    class TcpServerSocker : public TcpSocket
    {
    private:
        struct sockaddr_in m_clientSocketAddress;
        int m_clientSocket;
        struct pollfd m_pollClientRead;
        struct pollfd m_pollClientWrite;

    public:
        void waitClientReadyForWrite()
        {
            poll(&m_pollClientWrite, 1, -1);
        }

        void closeClient()
        {
            ::close(m_clientSocket);
        }

        virtual bool createSocket() override
        {
            int result;

            if (!TcpSocket::createSocket())
            {
                return false;
            }

            int value = 1;
            setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int));

            if(bind(
                m_socket,
                (struct sockaddr *) &m_targetSocketAddress,
                sizeof(m_targetSocketAddress)) < 0)
            {
                return false;
            }

            return listen(m_socket, 1) != -1;
        }

        bool accept()
        {
            auto clientSocketStructSize = sizeof(m_clientSocketAddress);

            m_clientSocket = ::accept(
                    m_socket,
                    (struct sockaddr *)&m_clientSocketAddress,
                    (socklen_t*)&clientSocketStructSize);

            return m_clientSocket != -1;
        }

        virtual bool sendBuffer(void* buffer, size_t bufferSize)
        {
            return send(
                    m_clientSocket,
                    buffer,
                    bufferSize,
                    MSG_WAITALL) > 0;
        }

        virtual bool receiveBuffer(void* buffer, size_t bufferSize, ssize_t* receivedBytes)
        {
            auto received = recv(
                    m_clientSocket,
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

#endif //TESTTASK_TCPSOCKETSERVER_H
