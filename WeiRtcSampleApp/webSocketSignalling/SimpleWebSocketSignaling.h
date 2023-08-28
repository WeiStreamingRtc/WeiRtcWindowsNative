#pragma once

// clang-format off
#include "pch.h"
#include "PeerConnection.h"
#include "signalling\SimpleSignalling.h"
#include "webSocketSignalling\SimpleWebSocketChannel.h"

// clang-format on

class WEI_EXPORT SimpleWebSocketSignaling : public SimpleSignalling {
public:
	SimpleWebSocketSignaling(std::string url);
	~SimpleWebSocketSignaling();

	void StartSignalling() override;

private:	
	SimpleWebSocketChannel _signallingChannel;
};
