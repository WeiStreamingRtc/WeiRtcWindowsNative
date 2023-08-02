// clang-format off
#include "pch.h"
#include "VideoTrackSource.h"
// clang-format on
namespace WeiRtc {

    VideoTrackSource::VideoTrackSource(
        winrt::Windows::UI::Xaml::UIElement pipCanvas)
        : _pipRender(pipCanvas),
        webrtc::VideoTrackSource(false) {}

    void VideoTrackSource::ConnectPip() {
        _videoCapturer.StartCapture();
        _videoCapturer.AddOrUpdateSink(const_cast<VideoRender*>(&_pipRender),
            rtc::VideoSinkWants());
    }

    // Hank: this is the call back that will be called when peer get established,
    // AddOrUpdateSink()
    rtc::VideoSourceInterface<webrtc::VideoFrame>* VideoTrackSource::source() {
        return &_videoCapturer;
    }

} //namespace WeiRtc {