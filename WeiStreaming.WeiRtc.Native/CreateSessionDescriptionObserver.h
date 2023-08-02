#pragma once
#include "PeerConnection.h"
#include "PeerConnectionEventHandler.h"
#include "api/rtc_error.h"

namespace WeiRtc {
    class CreateSessionDescriptionObserver
        : public webrtc::CreateSessionDescriptionObserver {
    private:
        PeerConnection* _peerConnection;

    public:
        CreateSessionDescriptionObserver(PeerConnection* peerConnection);
        void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
        void OnFailure(webrtc::RTCError error) override;
    };
} //namespace WeiRtc {