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

    for (std::vector<rtc::scoped_refptr<VideoTrackSource>>::iterator t = _videoTracks.begin(); t != _videoTracks.end(); ++t) {
        t->get()->Close();
        t->release();       
    }

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

    // Hank need to figure out how to mute audio:
    // audioSrc.get()->kMuted
    _audioTracks.push_back(audioTrack);
    return audioTrack;
}
rtc::scoped_refptr<webrtc::VideoTrackInterface>
PeerConnectionFactory::CreateVideoTrack(
    MediaTypes::VideoSourceType type,
    winrt::Windows::UI::Xaml::UIElement pipCanvas, const char* videoLabel) {
    _videoSource = new rtc::RefCountedObject<VideoTrackSource>(pipCanvas, type);
    auto videoTrack = _webrtcPeerConnectionFactoryInterface->CreateVideoTrack(
        videoLabel, _videoSource);

    _videoSource->ConnectPip();

    _videoTracks.push_back(_videoSource);
    return videoTrack;
}

}  // namespace WeiRtc