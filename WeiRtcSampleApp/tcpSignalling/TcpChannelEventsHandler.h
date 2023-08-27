#pragma once

struct TcpChannelEventsHandler {
    virtual ~TcpChannelEventsHandler(){};
    // virtual void OnTcpConnected(bool server);
    virtual void OnTcpMessage(std::string message) = 0;
    virtual void SendMessage(std::string message) = 0;
    // virtual void OnTcpError(winrt::hstring desc);
    // virtual void OnTcpClose();
};