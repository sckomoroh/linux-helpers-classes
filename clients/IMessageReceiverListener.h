#ifndef TESTTASK_IMESSAGERECEIVERLISTENER_H
#define TESTTASK_IMESSAGERECEIVERLISTENER_H

#include "../data/Message.h"

namespace clients
{
    class IMessageReceiverListener
    {
    public:
        virtual void onMessageReceived(data::Message *message) = 0;
    };
}

#endif //TESTTASK_IMESSAGERECEIVERLISTENER_H
