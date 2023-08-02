// clang-format off
#include "pch.h"
#include "PeerConnectionFactory.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/create_peerconnection_factory.h"

#include "VideoTrackSource.h"
// clang-format on

namespace WeiRtc {

    PeerConnectionFactory::PeerConnectionFactory() {
        _networkThread = rtc::Thread::CreateWithSocketServer();
        _networkThread->SetName("pc_network_thread", nullptr);
        _networkThread->Start();

        _workerThread = rtc::Thread::Create();
        _workerThread->SetName("pc_worker_thread", nullptr);
        _workerThread->Start();

        _signalingThread = rtc::Thread::Create();
        _signalingThread->SetName("pc_signaling_thread", nullptr);
        _signalingThread->Start();

        _webrtcPeerConnectionFactoryInterface = webrtc::CreatePeerConnectionFactory(
            _networkThread.get(), _workerThread.get(), _signalingThread.get(),
            nullptr /* default_adm */, webrtc::CreateBuiltinAudioEncoderFactory(),
            webrtc::CreateBuiltinAudioDecoderFactory(),
            webrtc::CreateBuiltinVideoEncoderFactory(),
            webrtc::CreateBuiltinVideoDecoderFactory(), nullptr /* audio_mixer */,
            nullptr /* audio_processing */);
    }

    PeerConnectionFactory::~PeerConnectionFactory() {
        _workerThread.release();
        _networkThread.release();
        _signalingThread.release();
    }

    rtc::scoped_refptr<webrtc::PeerConnectionInterface>
        PeerConnectionFactory::CreatePeerConnection(
            webrtc::PeerConnectionInterface::RTCConfiguration config,
            webrtc::PeerConnectionObserver* observer) {
        return _webrtcPeerConnectionFactoryInterface->CreatePeerConnection(
            config, nullptr, nullptr, observer);
    }
    rtc::scoped_refptr<webrtc::AudioTrackInterface>
        PeerConnectionFactory::CreateAudioTrack(const char* audioLabel) {

        auto audioSrc = _webrtcPeerConnectionFactoryInterface->CreateAudioSource(
            cricket::AudioOptions());
        auto audioTrack = _webrtcPeerConnectionFactoryInterface->CreateAudioTrack(
            audioLabel, audioSrc);

        //Hank need to figure out how to mute audio:
        //audioSrc.get()->kMuted
        return audioTrack;
    }
    rtc::scoped_refptr<webrtc::VideoTrackInterface>
        PeerConnectionFactory::CreateVideoTrack(MediaTypes::VideoSourceType type,
            winrt::Windows::UI::Xaml::UIElement pipCanvas, const char* videoLabel) {

        if (MediaTypes::VideoSourceType::Desktop == type) {
            _desktopSource = new rtc::RefCountedObject<DesktopTrackSource>(pipCanvas);
            auto videoTrack = _webrtcPeerConnectionFactoryInterface->CreateVideoTrack(
                videoLabel, _desktopSource);

            _desktopSource->ConnectPip();
            return videoTrack;
        }
        else
        {
            _videoSource = new rtc::RefCountedObject<VideoTrackSource>(pipCanvas);
            auto videoTrack = _webrtcPeerConnectionFactoryInterface->CreateVideoTrack(
                videoLabel, _videoSource);

            _videoSource->ConnectPip();
            return videoTrack;
        }
    }

} //namespace WeiRtc {