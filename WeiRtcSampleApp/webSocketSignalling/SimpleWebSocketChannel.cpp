#include "pch.h"

#include "webSocketSignalling/SimpleWebSocketChannel.h"
#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXUserAgent.h"

SimpleWebSocketChannel::SimpleWebSocketChannel(std::string url) { _url = url; }

SimpleWebSocketChannel::~SimpleWebSocketChannel() {

}

int SimpleWebSocketChannel::Start() {
	ix::initNetSystem();
	_webSocket.setUrl(_url);

	return 0;
}

void SimpleWebSocketChannel::OnWebSocketMessage(const ix::WebSocketMessagePtr& message)
{

}