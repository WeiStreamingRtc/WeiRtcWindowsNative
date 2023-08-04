#pragma once

// clang-format off
#include "pch.h"
#include "api/video/video_frame.h"
#include "api/video/video_sink_interface.h"
#include "modules/video_capture/video_capture.h"
#include "VideoSource.h"
// clang-format on
namespace WeiRtc {
struct WEI_EXPORT VideoCapturer
    : public rtc::VideoSinkInterface<webrtc::VideoFrame>,
      public VideoSource {
    virtual bool StartCapture();
    virtual void StopCapture();
    virtual void Dispose();
    virtual bool IsDesktopCapturer();
    virtual int GetVideoWidth();
    virtual int GetVideoHeight();

    void OnFrame(const webrtc::VideoFrame& frame) override;
};
}  // namespace WeiRtc