#pragma once

struct SignallingChannelMessageHandler {
    virtual ~SignallingChannelMessageHandler() {};
    virtual void OnMessage(std::string message) = 0;
};