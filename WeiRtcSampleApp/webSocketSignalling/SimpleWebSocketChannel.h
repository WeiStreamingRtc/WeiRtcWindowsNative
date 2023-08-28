#pragma once

#include "WinSock2.h"
#include "signalling\SignallingChannel.h"
#include "ixwebsocket/IXWebSocket.h"

class SimpleWebSocketChannel : public SignallingChannel {
   public:
    SimpleWebSocketChannel(std::string url);
    ~SimpleWebSocketChannel();
    int Start() override;
    void Send(std::string message) override;

   private:
    std::string _url;
    ix::WebSocket _webSocket;
};
