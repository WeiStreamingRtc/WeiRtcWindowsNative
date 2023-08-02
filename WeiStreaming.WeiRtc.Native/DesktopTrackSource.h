#pragma once

// clang-format off
#include "pch.h"
#include "pc/video_track_source.h"
#include "DesktopCapturer.h"
#include "VideoRender.h"
// clang-format on

namespace WeiRtc {

    class DesktopTrackSource : public webrtc::VideoTrackSource {
    public:
        DesktopTrackSource(winrt::Windows::UI::Xaml::UIElement pipCanvas);
        void ConnectPip();
        // Hank: this is the call back that will be called when peer get
        // established, AddOrUpdateSink()
        rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override;

    private:
        DesktopCapturer _capturer;
        const VideoRender _pipRender;
    };
} //namespace WeiRtc {
