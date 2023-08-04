// clang-format off
#include "pch.h"
#include "DesktopCapturer.h"
#include "modules/video_capture/video_capture_factory.h"
#include "libyuv.h"
#include "modules/desktop_capture/desktop_capturer.h"
// clang-format on

namespace WeiRtc {
    DesktopCapturer::DesktopCapturer() {
        _screenCapturer = new webrtc::ScreenCapturerWinGdi(webrtc::DesktopCaptureOptions::CreateDefault());

        webrtc::DesktopCapturer::SourceList list;
        auto source = _screenCapturer->GetSourceList(&list);
        auto select = _screenCapturer->SelectSource(list[1].id);
        _offset = _interval_ms * _nanoSecondPerMillSecond / 2;
        //_offset = 77088;
    }

    DesktopCapturer::~DesktopCapturer()
    {
        //release the capture resource here
        Dispose();
    }

    void DesktopCapturer::CaptureFrame()
    {
        _screenCapturer->CaptureFrame();
    }
    void DesktopCapturer::OnCaptureResult(webrtc::DesktopCapturer::Result result, std::unique_ptr<webrtc::DesktopFrame> frame)
    {
        if (result == webrtc::DesktopCapturer::Result::SUCCESS)
        {
            int width = frame->size().width();
            int height = frame->size().height();

            _width = width;
            _height = height;

            if (!_i420_buffer.get() ||
                _i420_buffer->width() * _i420_buffer->height() < width * height) {
                _i420_buffer = webrtc::I420Buffer::Create(width, height);
            }

            /* https://groups.google.com/g/discuss-webrtc/c/dFI65hUlscU
            if (!_i420_buffer.get() ||
                _i420_buffer->width() * _i420_buffer->height() < width * height) {
                _i420_buffer = webrtc::I420Buffer::Create(width, height);
            }
             libyuv::ConvertToI420(frame->data(), 0, _i420_buffer->MutableDataY(),
                            _i420_buffer->StrideY(), _i420_buffer->MutableDataU(),
                            _i420_buffer->StrideU(), _i420_buffer->MutableDataV(),
                            _i420_buffer->StrideV(), 0, 0, width, height, width,
                            height, libyuv::kRotate0, libyuv::FOURCC_ARGB);
            */

            libyuv::ConvertToI420(frame->data(), 0, _i420_buffer->StrideY(), _i420_buffer->MutableDataU(), _i420_buffer->StrideU(),
                _i420_buffer->MutableDataY(), _i420_buffer->StrideY(), _i420_buffer->MutableDataU(), _i420_buffer->StrideU(), _i420_buffer->MutableDataV(),
                _i420_buffer->StrideV(), 0, 0, width, height, width,
                height, libyuv::kRotate0, libyuv::FOURCC_ARGB);

            //auto currentTime = std::chrono::system_clock::now();
            //auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() + (_offset * _frameCount++) ;

            u_int64 timestamp = _startTime + (_frameCount++ * _offset);
            ::webrtc::VideoFrame::Builder new_frame_builder =
                ::webrtc::VideoFrame::Builder()
                .set_video_frame_buffer(_i420_buffer)
                .set_rotation(::webrtc::kVideoRotation_0)
                .set_timestamp_us(timestamp);

            OnFrame(new_frame_builder.build());

            /*Hank try to sclae the frame
            if (!_i420_buffer_out.get()) {
                _i420_buffer_out = webrtc::I420Buffer::Create(848, 480);
            }

                _i420_buffer_out->ScaleFrom(*originalFrame.video_frame_buffer()->ToI420());
                ::webrtc::VideoFrame::Builder scale_frame_builder =
                    ::webrtc::VideoFrame::Builder()
                    .set_video_frame_buffer(_i420_buffer_out)
                    .set_rotation(::webrtc::kVideoRotation_0)
                    .set_timestamp_us(originalFrame.timestamp_us());

                if (originalFrame.has_update_rect()) {
                    ::webrtc::VideoFrame::UpdateRect new_rect =
                        originalFrame.update_rect().ScaleWithFrame(
                            originalFrame.width(), originalFrame.height(), 0, 0, originalFrame.width(),
                            originalFrame.height(), 848, 480);
                    scale_frame_builder.set_update_rect(new_rect);
                }

            OnFrame(scale_frame_builder.build());
            */

            //OnFrame(webrtc::VideoFrame(_i420_buffer, webrtc::kVideoRotation_0, timeInSeconds));
        }
    }

    bool DesktopCapturer::StartCapture() {
        //Hacking for the ui is not ready but frame is ready
        std::this_thread::sleep_for(std::chrono::seconds(5));

        _screenCapturer->Start(this);
        _thread = std::thread(&DesktopCapturer::CaptureFrames, this);
        _isRunning = true;

        return true;
    }
    void DesktopCapturer::StopCapture() {

        _isRunning = false;
        if (_thread.joinable()) {
            _thread.join();
        }
    }
    void DesktopCapturer::Dispose() {
        StopCapture();
        delete _screenCapturer;
    }
    bool DesktopCapturer::IsDesktopCapturer() {
        return true;
    }
    int DesktopCapturer::GetVideoWidth() {
        return _width;
    }
    int DesktopCapturer::GetVideoHeight() {
        return _height;
    }

    void DesktopCapturer::CaptureFrames()
    {
        while (_isRunning) {
            CaptureFrame();
            std::this_thread::sleep_for(std::chrono::microseconds(_interval_ms));
        }
    }
} //namespace WeiRtc {