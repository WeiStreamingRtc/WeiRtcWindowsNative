// clang-format off
#include "pch.h"
#include "VideoCapturer.h"
#include "modules/video_capture/video_capture_factory.h"
// clang-format on
namespace WeiRtc {

    void VideoCapturer::OnFrame(const webrtc::VideoFrame& frame) {
        VideoSource::OnFrame(frame);
    }

    bool VideoCapturer::StartCapture() {
        return false;
    }
    void VideoCapturer::StopCapture() {}
    void VideoCapturer::Dispose() {}
    bool VideoCapturer::IsDesktopCapturer() { return false; }
    int VideoCapturer::GetVideoWidth() { return 0; }
    int VideoCapturer::GetVideoHeight() { return 0; }
} //namespace WeiRtc {