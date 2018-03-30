#ifndef TESTTASK_MESSAGE_H
#define TESTTASK_MESSAGE_H

namespace data
{
    typedef struct __Message
    {
        uint16_t MessageSize;
        uint8_t MessageType;
        uint64_t MessageId;
        uint64_t MessageData;
    } Message;
}

#endif //TESTTASK_MESSAGE_H
