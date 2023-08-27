#pragma once

#include "WinSock2.h"
#include "TcpChannelEventsHandler.h"

class DirectTcpServer {
   public:
    DirectTcpServer(int port);
    ~DirectTcpServer();
    int Start();
    void RegisterMessageCallBack(TcpChannelEventsHandler* handler);
    void Send(std::string message);

   private:
    void ListeningLoop();
    TcpChannelEventsHandler* _eventHandler;

   private:
    int _port;
    SOCKET _clientSocket = INVALID_SOCKET;
};
