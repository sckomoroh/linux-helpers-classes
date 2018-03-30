#ifndef TESTTASK_SOCKET_H
#define TESTTASK_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace network
{
    class Socket
    {
    protected:
        int m_socket;
        struct sockaddr_in m_targetSocketAddress;

    private:
        struct pollfd m_pollRead;
        struct pollfd m_pollWrite;

    protected:
        Socket()
            : m_socket(-1)
        {
        }

    public:
        void waitReadyForRead()
        {
            poll(&m_pollRead, 1, -1);
        }

        void waitReadyForWrite()
        {
            poll(&m_pollWrite, 1, -1);
        }

        void createTargetSocketAddress(const char* serverIp, uint16_t port)
        {
            m_targetSocketAddress.sin_family = AF_INET;
            m_targetSocketAddress.sin_port = htons(port);
            m_targetSocketAddress.sin_addr.s_addr = inet_addr(serverIp);
        }

        void close()
        {
            ::close(m_socket);
        }

        virtual bool createSocket() = 0;

        virtual bool sendBuffer(void* buffer, size_t bufferSize) = 0;

        virtual bool receiveBuffer(void* buffer, size_t bufferSize, ssize_t* receivedBytes) = 0;

    protected:
        bool setSocketAsyncFlag()
        {
            int socketFlags = fcntl(m_socket, F_GETFL);
            socketFlags |= O_NONBLOCK;
            return fcntl(m_socket, F_SETFL, socketFlags) > -1;
        }

        void createSocketPolls()
        {
            m_pollRead.fd = m_socket;
            m_pollRead.events = POLLIN;

            m_pollWrite.fd = m_socket;
            m_pollWrite.events = POLLOUT;
        }
    };
}

#endif //TESTTASK_SOCKET_H
