#pragma once
// clang-format off
#include "pch.h"
#include "api//video/video_frame.h"
#include "api/video/video_sink_interface.h"
#include "api/video/i420_buffer.h"
// clang-format on

namespace WeiRtc {

    class WEI_EXPORT VideoRender : public rtc::VideoSinkInterface<webrtc::VideoFrame> {
    public:
        VideoRender(winrt::Windows::UI::Xaml::UIElement canvas);
        ~VideoRender() = default;

        void OnFrame(const webrtc::VideoFrame& frame) override;

    private:
        winrt::Windows::Foundation::IAsyncAction VideoRendererAsync(
            winrt::Windows::UI::Xaml::UIElement canvas);
        winrt::Windows::Foundation::IAsyncAction PaintFrameAsync(
            std::unique_ptr<uint8_t[]> data, size_t length, int32_t width,
            int32_t height);

    private:
        const winrt::Windows::UI::Core::CoreDispatcher _uiThread;
        winrt::Microsoft::Graphics::Canvas::CanvasDevice _canvasDevice;
        winrt::Windows::UI::Composition::CompositionDrawingSurface _surface;
        rtc::scoped_refptr<webrtc::I420Buffer> _i420_buffer_out;
        int _canvasWidth;
        int _canvasHeight;
        int _desiredWidth;
        int _desiiredHeight;
        float _canvasRate;
        int32_t _canvasSize;
    };

} //namespace WeiRtc {