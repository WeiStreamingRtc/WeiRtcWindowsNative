// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

// clang-format off
#include "pch.h"
#include "WeiRtcApp.h"
#include "WeiRtcApp.g.cpp"

#include <stddef.h>
#include <memory>

#include "DirectTcpServer.h"
#include "SimpleTcpSignaling.h"
#include "TcpChannelEventsHandler.h"

#include "WeiRtc.h"
#include "CreateSessionDescriptionObserver.h"
#include "PeerConnection.h"
#include "PeerConnectionEventHandler.h"
#include "PeerConnectionFactory.h"
#include "PeerConnectionStates.h"
#include "SetSessionDescriptionObserer.h"
#include "CameraCapturer.h"
#include "VideoRender.h"
#include "VideoSource.h"
#include "MediaTypes.h"

// clang-format on

namespace winrt::WeiRtcSampleApp::implementation {

struct PeerConnectionOwner {
    virtual void OnPeerConnectionDropped() = 0;
};

struct PeerConnectionEventHandlerImpl : public WeiRtc::PeerConnectionEventHandler {
    SimpleTcpSignaling* _signaling;
    PeerConnectionOwner* _owner;
    PeerConnectionEventHandlerImpl(SimpleTcpSignaling* signaling,
                                   PeerConnectionOwner* owner)
        : _signaling(signaling), _owner(owner) {}

    void OnSdpCreated(const WeiRtc::SessionDescription& sdp) override {
        // Send the sdp
        std::string sdpStr = sdp.GetSdpAsString();
        winrt::Windows::Data::Json::JsonObject j_sdp;

        auto j_val = winrt::Windows::Data::Json::JsonValue::CreateStringValue(
            winrt::to_hstring(sdpStr));
        auto j_type =
            winrt::Windows::Data::Json::JsonValue::CreateStringValue(L"answer");
        j_sdp.Insert(L"sdp", j_val);
        j_sdp.Insert(L"type", j_type);

        _signaling->SendMessage((winrt::to_string(j_sdp.ToString())));
    }
    void OnSdpCreationFailed(const char* msg) override {}

    void OnSdpSetSuccess() override {}
    void OnSdpSetFailure(const char* msg) override {}

    void OnSignallingStateChanged(int state) override {}
    void OnStateChanged(int state) override {}

    void OnAddIceCandidateSuccess() override {}
    void OnAddIceCandidateFailure(const char* msg) override {}

    void OnIceCandidateStateChanged(int state) override {}
    void OnIceCandidateGatheringStateChanged(int state) override {}

    void OnIceCandidateGenerated(const WeiRtc::IceCandidate& cnd) override {
        // send the cnd
        _signaling->SendCandidate(cnd.GetCandidate());
    }

    void OnStandardizedIceConnectionChange(
        WeiRtc::PeerConnectionStates::IceConnectionState state) override {
        if (state ==
            WeiRtc::PeerConnectionStates::IceConnectionState::kIceConnectionFailed) {
            _owner->OnPeerConnectionDropped();
        }
    }
    void OnConnectionChange(
        WeiRtc::PeerConnectionStates::PeerConnectionState state) override {}
};
struct WebRtcSample : public PeerConnectionOwner {
    const char* _streamId = "stream_id";
    const char* _audioLabel = "audio_label";
    const char* _videoLabel = "video_label";


    SimpleTcpSignaling _signaling;

    PeerConnectionEventHandlerImpl _eventHandler =
        PeerConnectionEventHandlerImpl(&_signaling, this);

    WeiRtc::PeerConnectionFactory* _peerConnectionFactory = new WeiRtc::PeerConnectionFactory();

    std::unique_ptr<WeiRtc::PeerConnection> _pc;
    WebRtcSample(winrt::hstring room) : _signaling() {}

    void CreatePeerConnection(winrt::Windows::UI::Xaml::UIElement canvas) {
        webrtc::PeerConnectionInterface::RTCConfiguration config;
        config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
        config.enable_dtls_srtp = true;

        //_signaling.PopulateServers(config.servers);

        _pc.reset(new WeiRtc::PeerConnection(config, _peerConnectionFactory, canvas,
                                     &_eventHandler));

        _signaling.RegisterPeerConnection(_pc.get());
    }

    void StartSignalling() { _signaling.StartSignalling(); }

    void AddAudioTrack() {
        _pc->AddAudioTrack(WeiRtc::MediaTypes::AudioSourceType::Mic);
    }

    void AddVideoTrack(winrt::Windows::UI::Xaml::UIElement pipCanvas) {
        _pc->AddVideoTrack(WeiRtc::MediaTypes::VideoSourceType::Camera, pipCanvas);
    }

    void AddDesktopTrack(winrt::Windows::UI::Xaml::UIElement screenPipCanvas) {
        _pc->AddVideoTrack(WeiRtc::MediaTypes::VideoSourceType::Desktop, screenPipCanvas);
    }

    void OnPeerConnectionDropped() {
        // Client code should create a mapping, so it know which pc has dropped
        // and tear down that one

        _pc.get()->Close();
        delete _peerConnectionFactory;
    }
};

WeiRtcApp::WeiRtcApp() { WeiRtc::InitializeWeiRtc(); }

WeiRtcApp::~WeiRtcApp() {
    if (_sample != nullptr) {
        delete _sample;
    }

    WeiRtc::CleanupWeiRtc();
}

hstring WeiRtcApp::Room() const { return _room; }

Windows::Foundation::IAsyncAction WeiRtcApp::Init(
    Windows::UI::Xaml::UIElement canvas, Windows::UI::Xaml::UIElement pipCanvas,
    Windows::UI::Xaml::UIElement screenPipCanvas, hstring room) {
    if (room.empty()) {
        throw hresult_invalid_argument();
    }

    _room = room;
    _screenPipCanvas = &screenPipCanvas;

    // This is necessary to avoid STA
    co_await resume_background();

    _sample = new WebRtcSample(_room);

    _sample->CreatePeerConnection(canvas);

    _sample->AddAudioTrack();
    _sample->AddVideoTrack(pipCanvas);
    //_sample->AddDesktopTrack(*_screenPipCanvas);
    
    // Call this at the as the last step
    _sample->StartSignalling();
}

Windows::Foundation::IAsyncAction WeiRtcApp::StartDesktopCaptuer()
{
    co_await resume_background();
    _sample->AddDesktopTrack(*_screenPipCanvas);
}



}  // namespace winrt::WeiRtcSampleApp::implementation
   // namespace winrt::WeiRtcSampleApp::implementation
