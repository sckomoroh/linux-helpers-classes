#include <iostream>

#include "clients/MessageReceiverThread.h"
#include "clients/MessageServerThread.h"
#include "clients/MessageStorage.h"
#include "clients/StorageMessageClientThread.h"
#include "clients/StorageMessageServerThread.h"

using namespace clients;

#define IP_ADDRESS      "127.0.0.1"
#define MESSAGE_PORT    8888
#define STORAGE_PORT    8889

int main()
{
    MessageStorage storage;

    MessageReceiverThread receiverThread1;
    MessageReceiverThread receiverThread2;
    MessageServerThread serverThread;

    StorageMessageClientThread storageClient;
    StorageMessageServerThread storageServer;

    printf("Initializing message sender\n");
    if (!serverThread.initialize(IP_ADDRESS, MESSAGE_PORT))
    {
        auto errorCode = errno;
        auto errorDescription = strerror(errorCode);

        printf(
                "Unable to initialize the message server\n\tCode: %d\n\tDescription: %s\n",
                errorCode,
                errorDescription);

        return -1;
    }

    printf("Initializing message receiver 1\n");
    if (!receiverThread1.initialize(IP_ADDRESS, MESSAGE_PORT))
    {
        auto errorCode = errno;
        auto errorDescription = strerror(errorCode);

        printf(
                "Unable to initialize the message client\n\tCode: %d\n\tDescription: %s\n",
                errorCode,
                errorDescription);

        return -1;
    }

    printf("Initializing message receiver 2\n");
    if (!receiverThread2.initialize(IP_ADDRESS, MESSAGE_PORT))
    {
        auto errorCode = errno;
        auto errorDescription = strerror(errorCode);

        printf(
                "Unable to initialize the message client\n\tCode: %d\n\tDescription: %s\n",
                errorCode,
                errorDescription);

        return -1;
    }

    printf("Initializing storage server\n");
    if (!storageServer.initialize(IP_ADDRESS, STORAGE_PORT))
    {
        auto errorCode = errno;
        auto errorDescription = strerror(errorCode);

        printf(
                "Unable to initialize the storage server\n\tCode: %d\n\tDescription: %s\n",
                errorCode,
                errorDescription);

        return -1;
    }

    printf("Initializing storage client\n");
    if (!storageClient.initialize(IP_ADDRESS, STORAGE_PORT))
    {
        auto errorCode = errno;
        auto errorDescription = strerror(errorCode);

        printf(
                "Unable to initialize the storage client\n\tCode: %d\n\tDescription: %s\n",
                errorCode,
                errorDescription);

        return -1;
    }

    receiverThread1.addListener(&storage);
    receiverThread2.addListener(&storage);

    storage.addListener(&storageServer);

    printf("Start message server thread\n");
    serverThread.start();

    printf("Start message receiver threads\n");
    receiverThread1.start();
    receiverThread2.start();

    printf("Start storage server thread\n");
    storageServer.start();

    printf("Start storage client thread\n");
    storageClient.start();

    serverThread.join();
}