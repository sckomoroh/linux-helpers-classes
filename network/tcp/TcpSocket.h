#ifndef TESTTASK_TCPSOCKET_H
#define TESTTASK_TCPSOCKET_H

#include <fcntl.h>
#include <bits/poll.h>
#include "../Socket.h"

namespace network
{
    class TcpSocket : public Socket
    {
    public:
        virtual bool createSocket() override
        {
            m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (m_socket == -1)
            {
                return false;
            }

            createSocketPolls();

            return setSocketAsyncFlag();
        }
    };
}

#endif //TESTTASK_TCPSOCKET_H
