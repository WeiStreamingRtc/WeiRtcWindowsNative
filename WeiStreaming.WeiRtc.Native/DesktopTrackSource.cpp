// clang-format off
#include "pch.h"
#include "DesktopTrackSource.h"
// clang-format on

namespace WeiRtc {

    DesktopTrackSource::DesktopTrackSource(
        winrt::Windows::UI::Xaml::UIElement pipCanvas)
        : _pipRender(pipCanvas),
        webrtc::VideoTrackSource(false) {}

    void DesktopTrackSource::ConnectPip() {
        _capturer.AddOrUpdateSink(const_cast<VideoRender*>(&_pipRender),
            rtc::VideoSinkWants());

        _capturer.StartCapture();
    }

    // Hank: this is the call back that will be called when peer get established,
    // AddOrUpdateSink()
    rtc::VideoSourceInterface<webrtc::VideoFrame>* DesktopTrackSource::source() {
        return &_capturer;
    }
} //namespace WeiRtc {