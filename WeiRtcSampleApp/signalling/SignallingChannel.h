#pragma once

#include "SignallingChannelMessageHandler.h"
class SignallingChannel {

public:    
    virtual int Start();
    void RegisterMessageCallBack(SignallingChannelMessageHandler* handler);
    virtual void Send(std::string message);

protected:
    SignallingChannelMessageHandler* _messageHandler;
};