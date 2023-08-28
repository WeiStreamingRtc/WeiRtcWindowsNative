#pragma once

#include "WinSock2.h"
#include "signalling\SignallingChannel.h"

class DirectTcpServer : public SignallingChannel {
   public:
    DirectTcpServer(int port);
    ~DirectTcpServer();
    int Start() override;
    void Send(std::string message) override;

   private:
    void ListeningLoop();

   private:
    int _port;
    SOCKET _clientSocket = INVALID_SOCKET;
};
