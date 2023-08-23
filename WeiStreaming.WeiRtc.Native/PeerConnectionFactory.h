#pragma once

// clang-format off
#include "pch.h"
#include "api/peer_connection_interface.h"
#include "api/media_stream_interface.h"
#include "VideoTrackSource.h"
#include "MediaTypes.h"
// clang-format on

namespace WeiRtc {

    class WEI_EXPORT PeerConnectionFactory {
    public:
        PeerConnectionFactory();
        ~PeerConnectionFactory();

        rtc::scoped_refptr<webrtc::PeerConnectionInterface> CreatePeerConnection(
            webrtc::PeerConnectionInterface::RTCConfiguration config, webrtc::PeerConnectionObserver* observer);
        rtc::scoped_refptr<webrtc::AudioTrackInterface> CreateAudioTrack(
            const char* audioLabel);
        rtc::scoped_refptr<webrtc::VideoTrackInterface> CreateVideoTrack(MediaTypes::VideoSourceType type,
            winrt::Windows::UI::Xaml::UIElement pipCanvas, const char* videoLabel);

    private:
        std::unique_ptr<rtc::Thread> _networkThread;
        std::unique_ptr<rtc::Thread> _workerThread;
        std::unique_ptr<rtc::Thread> _signalingThread;
        rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
            _webrtcPeerConnectionFactoryInterface;
        VideoTrackSource* _videoSource;
        VideoTrackSource* _desktopSource;
    };

} //namespace WeiRtc {