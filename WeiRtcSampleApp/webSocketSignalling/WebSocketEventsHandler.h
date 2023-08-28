#pragma once

struct WebSocketEventsHandler {
    virtual ~WebSocketEventsHandler(){};    
    virtual void OnWebSocketMessage(std::string message) = 0;
};