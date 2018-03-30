cmake_minimum_required(VERSION 3.9)
project(TestTask)

set(CMAKE_CXX_STANDARD 11)

add_executable(TestTask main.cpp thread/Thread.h thread/Locker.h thread/AutoLocker.h network/Socket.h network/udp/UdpSocket.h network/tcp/TcpSocket.h network/udp/UdpClientSocket.h network/udp/UdpServerSocket.h data/Message.h clients/MessageReceiverThread.h clients/IMessageReceiverListener.h collections/Collection.h collections/Queue.h clients/MessageServerThread.h clients/MessageStorage.h clients/IMessageStorageListener.h network/tcp/TcpClientSocket.h network/tcp/TcpServerSocket.h clients/StorageMessageServerThread.h clients/StorageMessageClientThread.h)

target_link_libraries(TestTask pthread)