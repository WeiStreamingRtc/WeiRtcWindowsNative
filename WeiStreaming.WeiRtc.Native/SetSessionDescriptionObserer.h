#pragma once
#include "PeerConnection.h"
#include "PeerConnectionEventHandler.h"
#include "api/peer_connection_interface.h"
#include "api/rtc_error.h"

namespace WeiRtc {

    class SetSessionDescriptionObserver
        : public webrtc::SetSessionDescriptionObserver {
    public:
        SetSessionDescriptionObserver(PeerConnectionEventHandler* eventHandler);
        void OnSuccess() override;
        void OnFailure(webrtc::RTCError error) override;

    private:
        PeerConnectionEventHandler* _eventHandler;
    };

} //namespace WeiRtc {