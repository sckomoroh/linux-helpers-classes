#ifndef TESTTASK_UDPCLIENTSOCKET_H
#define TESTTASK_UDPCLIENTSOCKET_H

#include "UdpSocket.h"

namespace network
{
    namespace udp
    {
        class UdpClientSocket : public UdpSocket
        {
        public:
            virtual bool sendBuffer(void* buffer, size_t bufferSize)
            {
                return sendto(
                        m_socket,
                        buffer,
                        bufferSize,
                        MSG_WAITALL,
                        (struct sockaddr *) &m_targetSocketAddress,
                        sizeof(m_targetSocketAddress)) > 0;
            }

            virtual bool receiveBuffer(void* buffer, size_t bufferSize, ssize_t* receivedBytes)
            {
                socklen_t socketAddressStructSize = sizeof(m_targetSocketAddress);

                auto receiveResult = recvfrom(
                        m_socket,
                        buffer,
                        bufferSize,
                        MSG_WAITALL,
                        (struct sockaddr *) &m_targetSocketAddress,
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

#endif //TESTTASK_UDPCLIENTSOCKET_H
