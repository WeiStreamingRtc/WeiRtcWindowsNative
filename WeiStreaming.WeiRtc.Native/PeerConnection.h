#pragma once

// clang-format off
#include "pch.h"
#include "VideoRender.h"
#include "PeerConnectionFactory.h"
#include "SessionDescription.h"
#include "IceCandidate.h"
#include "PeerConnectionEventHandler.h"
#include "MediaTypes.h"
// clang-format on

namespace WeiRtc {

    class WEI_EXPORT PeerConnection : public webrtc::PeerConnectionObserver {
    public:

        PeerConnection(webrtc::PeerConnectionInterface::RTCConfiguration config,
            PeerConnectionFactory* factory,
            winrt::Windows::UI::Xaml::UIElement canvas,
            PeerConnectionEventHandler* eventHandler);
        ~PeerConnection();

        // Triggered when the SignalingState changed.
        void OnSignalingChange(
            webrtc::PeerConnectionInterface::SignalingState new_state) override;
        // Triggered when a remote peer opens a data channel.
        void OnDataChannel(
            rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;

        // Triggered when renegotiation is needed. For example, an ICE restart
        // has begun.
        void OnRenegotiationNeeded() override;

        // Called any time the IceGatheringState changes.
        void OnIceGatheringChange(
            webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

        // A new ICE candidate has been gathered.
        void OnIceCandidate(
            const webrtc::IceCandidateInterface* candidate) override;

        // Called any time the standards-compliant IceConnectionState changes.
        void OnStandardizedIceConnectionChange(
            webrtc::PeerConnectionInterface::IceConnectionState new_state) override;

        // Called any time the PeerConnectionState changes.
        void OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState
            new_state) override;

        // This is called when signaling indicates a transceiver will be receiving
        // media from the remote endpoint. This is fired during a call to
        // SetRemoteDescription. The receiving track can be accessed by:
        // |transceiver->receiver()->track()| and its associated streams by
        // |transceiver->receiver()->streams()|.
        // Note: This will only be called if Unified Plan semantics are specified.
        // This behavior is specified in section 2.2.8.2.5 of the "Set the
        // RTCSessionDescription" algorithm:
        // https://w3c.github.io/webrtc-pc/#set-description
        void OnTrack(rtc::scoped_refptr<webrtc::RtpTransceiverInterface>
            transceiver) override;

        webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpSenderInterface>> AddTrack(
            rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track,
            const std::vector<std::string>& stream_ids);
        rtc::scoped_refptr<webrtc::PeerConnectionInterface> AsInterface();

        void SetRemoteDescription(SessionDescription const& sdp);
        void SetLocalDescription(SessionDescription const& sdp);
        void CreateAnswer();
        void CreateOffer();
        bool AddIceCandidate(IceCandidate const& value);
        PeerConnectionEventHandler* GetPeerConnectionEventHander();

        //Hank: next step, return the sendinface, so client can get states, need to expose them
        void AddAudioTrack(MediaTypes::AudioSourceType type);
        void AddVideoTrack(MediaTypes::VideoSourceType type, winrt::Windows::UI::Xaml::UIElement pipCanvas);


        // Call this when done with the call, or detected connection has dropped to
        // clear up the resource.
        void Close();

    private:
        const char* _streamId = "wei_stream_id";
        const char* _audioLabel = "wei_audio_label";
        const char* _videoLabel = "wei_video_label";
        const VideoRender _videoRender;
        PeerConnectionFactory* _factory;
        rtc::scoped_refptr<webrtc::PeerConnectionInterface> _instance;
        PeerConnectionEventHandler* _eventHandler;

        std::vector<rtc::scoped_refptr<webrtc::AudioTrackInterface>> _audioTracks;
        std::vector<rtc::scoped_refptr<webrtc::VideoTrackInterface>> _videoTracks;
    };

} //namespace WeiRtc {