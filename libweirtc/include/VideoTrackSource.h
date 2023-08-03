#pragma once

// clang-format off
#include "pch.h"
#include "pc/video_track_source.h"
#include "CameraCapturer.h"
#include "VideoRender.h"
// clang-format on

namespace WeiRtc {

    class VideoTrackSource : public webrtc::VideoTrackSource {
    public:
        VideoTrackSource(winrt::Windows::UI::Xaml::UIElement pipCanvas);
        void ConnectPip();
        // Hank: this is the call back that will be called when peer get
        // established, AddOrUpdateSink()
        rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override;

    private:
        CameraCapturer _videoCapturer;
        const VideoRender _pipRender;
    };

} //namespace WeiRtc {