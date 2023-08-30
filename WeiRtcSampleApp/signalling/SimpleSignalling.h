#pragma once

// clang-format off
#include "pch.h"
#include "PeerConnection.h"
#include "SignallingChannelMessageHandler.h"
#include "SignallingChannel.h"

// clang-format on

class WEI_EXPORT SimpleSignalling : public SignallingChannelMessageHandler {
public:
	SimpleSignalling();
	~SimpleSignalling();

	void RegisterPeerConnection(WeiRtc::PeerConnection* peerConnection);
	virtual void StartSignalling();

	void OnMessage(std::string message) override;
	void OnChannelOpen() override;
	void SendMessage(std::string message);

	void SendOffer(winrt::hstring sdp) const {}
	
	void SendAnswer(std::string sdpStr);
	
	void SendCandidate(std::string candidate);

	void SetSignallingChannel(SignallingChannel* channel);

	void CallSupport(winrt::hstring msg);

	std::string GetClientId();
	std::string GetDestId();

private:
	WeiRtc::PeerConnection* _peerConnection;
	SignallingChannel* _channel;
	std::string _clientId;
	std::string _destId;
};
