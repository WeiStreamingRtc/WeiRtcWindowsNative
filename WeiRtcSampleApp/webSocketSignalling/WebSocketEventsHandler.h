#pragma once

#include "ixwebsocket/IXWebSocketMessage.h"

struct WebSocketEventsHandler {
    virtual ~WebSocketEventsHandler(){};    
    virtual void OnWebSocketMessage(std::string message) = 0;
};