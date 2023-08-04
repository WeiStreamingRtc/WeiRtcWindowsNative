#pragma once

// clang-format off
#include "pch.h"
#include "pc/video_track_source.h"
#include "VideoCapturer.h"
#include "VideoRender.h"
#include "MediaTypes.h"
// clang-format on

namespace WeiRtc {

    class VideoTrackSource : public webrtc::VideoTrackSource {
    public:
        VideoTrackSource(winrt::Windows::UI::Xaml::UIElement pipCanvas, MediaTypes::VideoSourceType type);
        ~VideoTrackSource();
        void ConnectPip();
        rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override;

    private:
        VideoCapturer* _videoCapturer;
        const VideoRender _pipRender;
    };

} //namespace WeiRtc {