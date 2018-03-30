#ifndef TESTTASK_UDPSERVERSOCKET_H
#define TESTTASK_UDPSERVERSOCKET_H

#include "UdpSocket.h"

namespace network
{
    namespace udp
    {
        class UdpServerSocket : public UdpSocket
        {
        private:
            struct sockaddr_in m_clientSocketAddress;

        public:
            virtual bool createSocket() override
            {
                if(!UdpSocket::createSocket())
                {
                    return false;
                }

                return bind(
                        m_socket,
                        (struct sockaddr *) &m_targetSocketAddress,
                        sizeof(m_targetSocketAddress)) > -1;
            }

            virtual bool sendBuffer(void* buffer, size_t bufferSize)
            {
                return sendto(
                        m_socket,
                        buffer,
                        bufferSize,
                        MSG_WAITALL,
                        (struct sockaddr *) &m_clientSocketAddress,
                        sizeof(struct sockaddr_in)) > 0;
            }

            virtual bool receiveBuffer(void* buffer, size_t bufferSize, ssize_t* receivedBytes)
            {
                socklen_t socketAddressStructSize = sizeof(struct sockaddr_in);

                auto receiveResult = recvfrom(
                        m_socket,
                        buffer,
                        bufferSize,
                        MSG_WAITALL,
                        (struct sockaddr *) &m_clientSocketAddress,
                        &socketAddressStructSize);

                if (receivedBytes != nullptr)
                {
                    *receivedBytes = receiveResult;
                }

                return receiveResult > 0;
            }
        };
    }
}

#endif //TESTTASK_UDPSERVERSOCKET_H
