#pragma once

// clang-format off
#include "pch.h"
#include "PeerConnection.h"
#include "signalling\SimpleSignalling.h"
#include "DirectTcpServer.h"
// clang-format on

class WEI_EXPORT SimpleTcpSignaling : public SimpleSignalling {
public:
	SimpleTcpSignaling();
	~SimpleTcpSignaling();

	void StartSignalling() override;

private:
	DirectTcpServer _tcpSignallingServer;	
};
