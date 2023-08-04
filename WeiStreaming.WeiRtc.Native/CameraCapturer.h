#pragma once

// clang-format off
#include "pch.h"
#include "modules/video_capture/video_capture.h"
#include "VideoCapturer.h"
// clang-format on

namespace WeiRtc {

    // this is a member of VideoTrackSource, which is created and add to peer
    // connection in AddVideoTrack()

    class CameraCapturer : public VideoCapturer {
    public:
        CameraCapturer();
        ~CameraCapturer();

        bool StartCapture() override;
        void StopCapture() override;
        void Dispose() override;
        bool IsDesktopCapturer() override;
        int GetVideoWidth() override;
        int GetVideoHeight() override;

    private:
        rtc::scoped_refptr<webrtc::VideoCaptureModule> _videoCaptureModule;
        webrtc::VideoCaptureCapability _capability;
    };

} //namespace WeiRtc {