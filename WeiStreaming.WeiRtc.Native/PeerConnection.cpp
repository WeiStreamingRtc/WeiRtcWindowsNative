// clang-format off
#include "pch.h"
#include "PeerConnection.h"
#include "SetSessionDescriptionObserer.h"
#include "CreateSessionDescriptionObserver.h"
// clang-format on

namespace WeiRtc {

    PeerConnection::PeerConnection(
        webrtc::PeerConnectionInterface::RTCConfiguration config,
        PeerConnectionFactory* factory, winrt::Windows::UI::Xaml::UIElement canvas,
        PeerConnectionEventHandler* eventHandler)
        : _videoRender(canvas) {
        _factory = factory;
        _instance = _factory->CreatePeerConnection(config, this);
        _eventHandler = eventHandler;
    }

    PeerConnection::~PeerConnection() {
        Close();
    }

    void PeerConnection::Close() {
        //Hank: TODO: looks like that webrtc will never comeout of this function.
        //_instance->Close();
    }

    //Hank: This is called by webrtc, since this class extends  webrtc::PeerConnectionObserver
    void PeerConnection::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
        std::string cndStr;
        candidate->ToString(&cndStr);

        IceCandidate cnd = IceCandidate(cndStr);
        _eventHandler->OnIceCandidateGenerated(cnd);
    }

    //Hank: call back when remote are adding a track.
    void PeerConnection::OnTrack(
        rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver) {
        rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track =
            transceiver->receiver()->track();
        if (track->kind() == webrtc::MediaStreamTrackInterface::kVideoKind) {

            //Hank: Connect the video to the renderer.
            static_cast<webrtc::VideoTrackInterface*>(track.get())
                ->AddOrUpdateSink(const_cast<VideoRender*>(&_videoRender),
                    rtc::VideoSinkWants());
        }

        //Hank: this is random string:  
        // auto id = track->id();    
        //Hank: here, we can choose to mute or unmute a remote track
        //track->set_enabled()
    }

    webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpSenderInterface>>
        PeerConnection::AddTrack(
            rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track,
            const std::vector<std::string>& stream_ids) {
        return _instance->AddTrack(track, stream_ids);
    }

    rtc::scoped_refptr<webrtc::PeerConnectionInterface>
        PeerConnection::AsInterface() {
        return _instance;
    }
    void PeerConnection::OnSignalingChange(
        webrtc::PeerConnectionInterface::SignalingState new_state) {}

    void PeerConnection::OnDataChannel(
        rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) {}

    void PeerConnection::OnRenegotiationNeeded() {}

    void PeerConnection::OnIceGatheringChange(
        webrtc::PeerConnectionInterface::IceGatheringState new_state) {}

    // Hank: OnStandardizedIceConnectionChange is called before OnConnectionChange when other part is hangup.
    // kIceConnectionDisconnected is followed with kIceConnectionFailed
    void PeerConnection::OnStandardizedIceConnectionChange(
        webrtc::PeerConnectionInterface::IceConnectionState new_state) {
        _eventHandler->OnStandardizedIceConnectionChange((PeerConnectionStates::IceConnectionState)new_state);
    }

    void PeerConnection::OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) {
        _eventHandler->OnConnectionChange((PeerConnectionStates::PeerConnectionState)new_state);
    }
    bool PeerConnection::AddIceCandidate(IceCandidate const& value) {

        webrtc::IceCandidateInterface* ice_candidate =
            webrtc::CreateIceCandidate(value.GetID(), value.GetIndex(), value.GetCandidate(), nullptr);
        assert(ice_candidate);
        return _instance->AddIceCandidate(ice_candidate);
    }

    void PeerConnection::SetRemoteDescription(SessionDescription const& sdp) {
        _instance->SetRemoteDescription(new rtc::RefCountedObject<SetSessionDescriptionObserver>(_eventHandler), sdp.GetIntance());
    }
    void PeerConnection::SetLocalDescription(SessionDescription const& sdp) {
        _instance->SetRemoteDescription(new rtc::RefCountedObject<SetSessionDescriptionObserver>(_eventHandler), sdp.GetIntance());
    }
    void PeerConnection::CreateAnswer() {
        //Hank: default option for now
        _instance->CreateAnswer(new rtc::RefCountedObject<CreateSessionDescriptionObserver>(this), webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }
    void PeerConnection::CreateOffer() {
        //Hank: default option for now
        _instance->CreateOffer(new rtc::RefCountedObject<CreateSessionDescriptionObserver>(this), webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }

    PeerConnectionEventHandler* PeerConnection::GetPeerConnectionEventHander()
    {
        return _eventHandler;
    }

    void PeerConnection::AddAudioTrack(MediaTypes::AudioSourceType type) {

        //Hank: ignore the type for now.
        auto track = _factory->CreateAudioTrack(_audioLabel);

        auto ret = AddTrack(track, { _streamId });
        assert(ret.ok());

        _audioTracks.push_back(track);
    }
    void PeerConnection::AddVideoTrack(MediaTypes::VideoSourceType type, winrt::Windows::UI::Xaml::UIElement pipCanvas) {

        auto track = _factory->CreateVideoTrack(type, pipCanvas, _videoLabel);

        auto ret = AddTrack(track, { _streamId });
        assert(ret.ok());
        _videoTracks.push_back(track);
    }


    /*void PeerConnection::SetRemoteDescription(
        SessionDescription const& sdp,
        const PeerConnectionSetSdpSuccessCallbak successCallback,
        const PeerConnectionSetSdpFailureCallback failureCallback) {

        _instance->SetRemoteDescription(new rtc::RefCountedObject<SetSessionDescriptionObserver>(successCallback, failureCallback), sdp.GetIntance());
    }
    void PeerConnection::SetLocalDescription(SessionDescription const& sdp,
        const PeerConnectionSetSdpSuccessCallbak successCallback,
        const PeerConnectionSetSdpFailureCallback failureCallback) {

        _instance->SetLocalDescription(new rtc::RefCountedObject<SetSessionDescriptionObserver>(successCallback, failureCallback), sdp.GetIntance());
    }
    void PeerConnection::CreateAnswer(const PeerConnectionCreateSdpSuccessCallback successCallback,
        const PeerConnectionCreateSdpFailureCallback failureCallback){
        //Hank: ignoring option for now
        _instance->CreateAnswer(new rtc::RefCountedObject<CreateSessionDescriptionObserver>(successCallback, failureCallback), webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }
    void PeerConnection::CreateOffer(const PeerConnectionCreateSdpSuccessCallback successCallback,
        const PeerConnectionCreateSdpFailureCallback failureCallback) {
        //Hank: ignoring option for now
        _instance->CreateOffer(new rtc::RefCountedObject<CreateSessionDescriptionObserver>(successCallback, failureCallback), webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }*/

} //namespace WeiRtc {