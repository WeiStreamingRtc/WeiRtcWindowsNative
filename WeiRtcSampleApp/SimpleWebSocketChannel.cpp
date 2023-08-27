#include "pch.h"

#include <iostream>
#include "webSocketSignalling/SimpleWebSocketChannel.h"
#include "webSocketSignalling/WebSocketEventsHandler.h"
#include "webSocketSignalling/SimpleWebSocketSignaling.h"
#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXUserAgent.h"

SimpleWebSocketChannel::SimpleWebSocketChannel(std::string url) { _url = url; }

SimpleWebSocketChannel::~SimpleWebSocketChannel() {

}

int SimpleWebSocketChannel::Start() {
	ix::initNetSystem();
	_webSocket.setUrl(_url);

	_webSocket.setOnMessageCallback([&](const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Message)
            {
                std::cout << "received message: " << msg->str << std::endl;
                std::cout << "> " << std::flush;

                _eventHandler->OnWebSocketMessage(msg->str);

            }
            else if (msg->type == ix::WebSocketMessageType::Open)
            {
                std::cout << "Connection established" << std::endl;
                std::cout << "> " << std::flush;
            }
            else if (msg->type == ix::WebSocketMessageType::Error)
            {
                // Maybe SSL is not configured properly
                std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
                std::cout << "> " << std::flush;
            }
        }
    );

    _webSocket.start();
	return 0;
}

void SimpleWebSocketChannel::RegisterMessageCallBack(WebSocketEventsHandler* handler)
{
    this->_eventHandler = handler;
}

void SimpleWebSocketChannel::Send(std::string message)
{
    _webSocket.send(message);
}
