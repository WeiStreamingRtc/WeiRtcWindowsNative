#pragma once

// clang-format off
#include "pch.h"
#include <chrono>
#include <thread>

#include <wingdi.h>

#include "api/video/video_frame.h"
#include "api/video/video_sink_interface.h"
#include "api/video/i420_buffer.h"
#include "modules/video_capture/video_capture.h"
#include "modules/desktop_capture/desktop_capturer.h"
#include "modules/desktop_capture/desktop_and_cursor_composer.h"
#include "modules/desktop_capture/win/screen_capturer_win_gdi.h"
#include "modules/desktop_capture/desktop_capture_types.h"
#include "modules/desktop_capture/desktop_frame.h"
#include "modules/desktop_capture/desktop_capture_options.h"
#include "modules/desktop_capture/desktop_capture_types.h"
#include "VideoSource.h"
#include "VideoCapturer.h"
// clang-format on

namespace WeiRtc {

    class DesktopCapturer : public VideoCapturer, public rtc::VideoSinkInterface<webrtc::VideoFrame>,
        public VideoSource, public webrtc::DesktopCapturer::Callback {
    public:
        DesktopCapturer();
        ~DesktopCapturer();
        void OnFrame(const webrtc::VideoFrame& frame) override;
        void OnCaptureResult(webrtc::DesktopCapturer::Result result, std::unique_ptr<webrtc::DesktopFrame> frame) override;
        void CaptureFrame();

        bool StartCapture() override;
        void StopCapture() override;
        void Dispose() override;
        bool IsDesktopCapturer() override;
        int GetVideoWidth() override;
        int GetVideoHeight() override;

    private:
        void CaptureFrames();
        webrtc::ScreenCapturerWinGdi* _screenCapturer;
        rtc::scoped_refptr<webrtc::I420Buffer> _i420_buffer;
        rtc::scoped_refptr<webrtc::I420Buffer> _i420_buffer_out;
        std::thread _thread;
        bool _isRunning = false;
        int _width;
        int _height;

        u_int64 _startTime = 690638571000;
        u_int64 _frameCount = 0;
        u_int64 _nanoSecondPerMillSecond = 1000000;
        int _interval_ms = 100;
        u_int64 _offset;
    };
} //namespace WeiRtc {