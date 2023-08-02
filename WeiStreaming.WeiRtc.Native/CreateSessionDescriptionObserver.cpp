#include "pch.h"

#include "CreateSessionDescriptionObserver.h"
#include "SetSessionDescriptionObserer.h"
#include "SessionDescription.h"

namespace WeiRtc {

    CreateSessionDescriptionObserver::CreateSessionDescriptionObserver(
        PeerConnection* peerConnection) {
        _peerConnection = peerConnection;
    }

    void CreateSessionDescriptionObserver::OnSuccess(
        webrtc::SessionDescriptionInterface* desc) {
        _peerConnection->AsInterface()->SetLocalDescription(
            new rtc::RefCountedObject<SetSessionDescriptionObserver>(_peerConnection->GetPeerConnectionEventHander()),
            desc);

        std::string sdp;
        desc->ToString(&sdp);

        webrtc::SdpType rtcType = desc->GetType();
        SessionDescription::SdpType weiType;
        switch (rtcType) {
        case webrtc::SdpType::kOffer:
            weiType = SessionDescription::SdpType::kOffer;
            break;
        case webrtc::SdpType::kAnswer:
            weiType = SessionDescription::SdpType::kAnswer;
            break;
        case webrtc::SdpType::kPrAnswer:
            weiType = SessionDescription::SdpType::kPrAnswer;
            break;
        case webrtc::SdpType::kRollback:
            weiType = SessionDescription::SdpType::kRollback;
            break;
        }

        SessionDescription sdpObj(weiType, sdp);
        _peerConnection->GetPeerConnectionEventHander()->OnSdpCreated(sdpObj);

        /*
        winrt::Windows::Data::Json::JsonObject j_sdp;

        auto j_val = winrt::Windows::Data::Json::JsonValue::CreateStringValue(
            winrt::to_hstring(sdp));
        auto j_type =
            winrt::Windows::Data::Json::JsonValue::CreateStringValue(L"answer");
        j_sdp.Insert(L"sdp", j_val);
        j_sdp.Insert(L"type", j_type);

        _signaling->SendMessage((winrt::to_string(j_sdp.ToString())));
        */
    }

    void CreateSessionDescriptionObserver::OnFailure(webrtc::RTCError error) {}

} //namespace WeiRtc {