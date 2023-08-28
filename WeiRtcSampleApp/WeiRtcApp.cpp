// clang-format off
#include "pch.h"

#include "WeiRtcApp.h"
#include "WeiRtcApp.g.cpp"

#include <stddef.h>
#include <memory>

#include "tcpSignalling/DirectTcpServer.h"
#include "tcpSignalling/SimpleTcpSignaling.h"

#include "WeiRtc.h"
#include "CreateSessionDescriptionObserver.h"
#include "PeerConnection.h"
#include "PeerConnectionEventHandler.h"
#include "PeerConnectionFactory.h"
#include "PeerConnectionStates.h"
#include "SetSessionDescriptionObserer.h"
#include "VideoRender.h"
#include "VideoSource.h"
#include "MediaTypes.h"

#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXUserAgent.h"
#include <iostream>

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

    void StartWebSocket() {
        // Required on Windows
        ix::initNetSystem();

        // Our websocket object
        ix::WebSocket webSocket;

        // Connect to a server with encryption
        // See https://machinezone.github.io/IXWebSocket/usage/#tls-support-and-configuration
        //     https://github.com/machinezone/IXWebSocket/issues/386#issuecomment-1105235227 (self signed certificates)
        std::string url("ws://localhost:8889/ws");
        webSocket.setUrl(url);

        std::cout << "Connecting to " << url << "..." << std::endl;

        // Setup a callback to be fired (in a background thread, watch out for race conditions !)
        // when a message or an event (open, close, error) is received
        webSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
            {
                if (msg->type == ix::WebSocketMessageType::Message)
                {
                    std::cout << "received message: " << msg->str << std::endl;
                    std::cout << "> " << std::flush;
                }
                else if (msg->type == ix::WebSocketMessageType::Open)
                {
                    std::cout << "Connection established" << std::endl;
                    std::cout << "> " << std::flush;
                }
                else if (msg->type == ix::WebSocketMessageType::Error)
                {
                    // Maybe SSL is not configured properly
                    std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
                    std::cout << "> " << std::flush;
                }
            }
        );

        // Now that our callback is setup, we can start our background thread and receive messages
        webSocket.start();

        // Send a message to the server (default to TEXT mode)
        webSocket.send("hello world");

        // Display a prompt
        std::cout << "> " << std::flush;

        std::string text;
        // Read text from the console and send messages in text mode.
        // Exit with Ctrl-D on Unix or Ctrl-Z on Windows.
        int n = 0;
        char* buffer = new char[100];

        while (true)
        {
            n++;
            Sleep(100);
            sprintf(buffer,"test %d", n);
            webSocket.send(buffer);
            std::cout << "> " << std::flush;
        }
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
    //_sample->AddVideoTrack(pipCanvas);
    _sample->AddDesktopTrack(*_screenPipCanvas);
    
    // Call this at the as the last step
    _sample->StartSignalling();
    //_sample->StartWebSocket();
}

Windows::Foundation::IAsyncAction WeiRtcApp::StartDesktopCaptuer()
{
    co_await resume_background();
    _sample->AddDesktopTrack(*_screenPipCanvas);
}



}  // namespace winrt::WeiRtcSampleApp::implementation
   // namespace winrt::WeiRtcSampleApp::implementation
