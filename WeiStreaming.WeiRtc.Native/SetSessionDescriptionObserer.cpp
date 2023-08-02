#include "pch.h"
#include "SetSessionDescriptionObserer.h"

namespace WeiRtc {

    SetSessionDescriptionObserver::SetSessionDescriptionObserver(PeerConnectionEventHandler* eventHandler) {

        _eventHandler = eventHandler;
    }

    void SetSessionDescriptionObserver::OnSuccess() {
        _eventHandler->OnSdpSetSuccess();
    }
    void SetSessionDescriptionObserver::OnFailure(webrtc::RTCError error) {
        _eventHandler->OnSdpSetFailure(error.message());
    }


    /*SetSessionDescriptionObserver::SetSessionDescriptionObserver(
        const PeerConnectionSetSdpSuccessCallbak successCallback,
        const PeerConnectionSetSdpFailureCallback failureCallback)
        : _successCallback(successCallback), _failureCallback(failureCallback) {
    }

    void SetSessionDescriptionObserver::OnSuccess() {
        WEI_CALL(_successCallback);
    }
    void SetSessionDescriptionObserver::OnFailure(webrtc::RTCError error) {
        WEI_CALL(_failureCallback, error.message());
    } */

} //namespace WeiRtc {