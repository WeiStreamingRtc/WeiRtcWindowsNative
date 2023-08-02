#pragma once

// clang-format off
#include "pch.h"
#include "PeerConnection.h"
#include "TcpChannelEventsHandler.h"
#include "DirectTcpServer.h"
// clang-format on

class WEI_EXPORT SimpleTcpSignaling : public TcpChannelEventsHandler {
public:
	SimpleTcpSignaling();
	~SimpleTcpSignaling();

	void RegisterPeerConnection(WeiRtc::PeerConnection* peerConnection);
	void StartSignalling();

	void OnTcpMessage(std::string message) override;
	void SendMessage(std::string message) override;

	void SendOffer(winrt::hstring sdp) const {}
	void SendCandidate(std::string candidate);

private:
	WeiRtc::PeerConnection* _peerConnection;
	DirectTcpServer _tcpSignallingServer;	
};
