#pragma once

#include "WinSock2.h"
#include "webSocketSignalling\WebSocketEventsHandler.h"
#include "ixwebsocket/IXWebSocket.h"

class SimpleWebSocketChannel {
   public:
    SimpleWebSocketChannel(std::string url);
    ~SimpleWebSocketChannel();
    int Start();
    void RegisterMessageCallBack(WebSocketEventsHandler* handler);
    void Send(std::string message);

   private:
    std::string _url;
    ix::WebSocket _webSocket;
    WebSocketEventsHandler* _eventHandler;
};
