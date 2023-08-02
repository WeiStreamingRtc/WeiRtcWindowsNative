// clang-format off
#include "pch.h"
#include "CameraCapturer.h"
#include "modules/video_capture/video_capture_factory.h"
#include "api/video/i420_buffer.h"
// clang-format on
namespace WeiRtc {

    CameraCapturer::CameraCapturer() {
        std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> device_info(
            webrtc::VideoCaptureFactory::CreateDeviceInfo());
        // Returns the id for the first camera connected to the computer.
        char id[256];
        device_info->GetDeviceName(0, nullptr, 0, id, sizeof(id), nullptr, 0);
        // Sets up camera and starts capturing.
        _videoCaptureModule = webrtc::VideoCaptureFactory::Create(id);

        webrtc::VideoCaptureCapability capability;
        // Hank: need to walk through, pick one video type that is has 2 plane
        //  VideoCaptureWinRTInternal::FrameArrived() at video_capture_winrt.cc
        //  expecting the picture format has 2 planes. one planes format are:
        //  YUY2

        int caps = device_info->NumberOfCapabilities(id);

        for (int n = 0; n < caps; n++) {
            device_info->GetCapability(id, n, capability);
            if (capability.videoType != webrtc::VideoType::kYUY2 &&
                capability.videoType != webrtc::VideoType::kUnknown) {
                break;
            }
        }
        _capability = capability;

        device_info.reset();
    }

    CameraCapturer::~CameraCapturer() {
        Dispose();
        //_videoCaptureModule is rtc::scoped_refptr, no need to worry about delete it, you cannot also
    }

    void CameraCapturer::OnFrame(const webrtc::VideoFrame& frame) {
        VideoSource::OnFrame(frame);
    }
    bool CameraCapturer::StartCapture() {
        _videoCaptureModule->RegisterCaptureDataCallback(this);
        _videoCaptureModule->StartCapture(_capability);
        return _videoCaptureModule->CaptureStarted();
    }
    void CameraCapturer::StopCapture() {
        _videoCaptureModule->StopCapture();
        _videoCaptureModule->DeRegisterCaptureDataCallback();
    }
    void CameraCapturer::Dispose() {
        StopCapture();
    }
    bool CameraCapturer::IsDesktopCapturer() {
        return false;
    }
    int CameraCapturer::GetVideoWidth() {
        return _capability.width;
    }
    int CameraCapturer::GetVideoHeight() {
        return _capability.height;
    }

} //namespace WeiRtc {