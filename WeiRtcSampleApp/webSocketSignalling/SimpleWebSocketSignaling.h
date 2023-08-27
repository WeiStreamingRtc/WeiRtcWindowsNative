#pragma once

// clang-format off
#include "pch.h"
#include "PeerConnection.h"
#include "webSocketSignalling\WebSocketEventsHandler.h"
#include "webSocketSignalling\SimpleWebSocketChannel.h"

// clang-format on

class WEI_EXPORT SimpleWebSocketSignaling : public WebSocketEventsHandler {
public:
	SimpleWebSocketSignaling(std::string url);
	~SimpleWebSocketSignaling();

	void RegisterPeerConnection(WeiRtc::PeerConnection* peerConnection);
	void StartSignalling();

	void OnWebSocketMessage(std::string message) override;
	void SendMessage(std::string message);

	void SendOffer(winrt::hstring sdp) const {}
	void SendCandidate(std::string candidate);

private:
	WeiRtc::PeerConnection* _peerConnection;
	SimpleWebSocketChannel _signallingChannel;
};
