#ifndef TESTTASK_UDPSOCKET_H
#define TESTTASK_UDPSOCKET_H

#include <fcntl.h>
#include "../Socket.h"

namespace network
{
    namespace udp
    {
        class UdpSocket : public Socket
        {

        protected:
            UdpSocket() = default;

        public:
            virtual bool createSocket() override
            {
                m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                if (m_socket == -1)
                {
                    return false;
                }

                createSocketPolls();

                return setSocketAsyncFlag();
            }
        };
    }
}

#endif //TESTTASK_UDPSOCKET_H
