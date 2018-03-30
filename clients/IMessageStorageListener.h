#ifndef TESTTASK_IMESSAGESTORAGELISTENER_H
#define TESTTASK_IMESSAGESTORAGELISTENER_H

#include "../data/Message.h"

namespace clients
{
    class IMessageStorageListener
    {
    public:
        virtual void onGeneralMessageReceived(data::Message *message) = 0;

        virtual void onClientMessageReceived(data::Message *message) = 0;
    };
}

#endif //TESTTASK_IMESSAGESTORAGELISTENER_H
