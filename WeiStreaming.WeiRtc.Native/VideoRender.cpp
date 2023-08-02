// clang-format off
#include "pch.h"
#include "VideoRender.h"
#include "api/video/i420_buffer.h"
#include "libyuv.h"
// clang-format on

namespace WeiRtc {

    VideoRender::VideoRender(winrt::Windows::UI::Xaml::UIElement canvas)
        : _uiThread(canvas.Dispatcher()), _surface(nullptr) {

        winrt::Windows::Foundation::Size size;

        // Run the operation on the UI thread
        _uiThread.RunAsync(
            winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, [&]() {
                size = canvas.RenderSize();
            }).get();

            _canvasWidth = size.Width;
            _canvasHeight = size.Height;

            _desiredWidth = _canvasWidth;
            _desiiredHeight = _canvasHeight;

            _canvasSize = _canvasWidth * _canvasHeight;
            _canvasRate = _canvasWidth / _canvasHeight;

            VideoRendererAsync(canvas);
    }

    winrt::Windows::Foundation::IAsyncAction VideoRender::VideoRendererAsync(
        winrt::Windows::UI::Xaml::UIElement canvas) {

        co_await winrt::resume_foreground(_uiThread);

        winrt::Windows::UI::Composition::Compositor compositor =
            winrt::Windows::UI::Xaml::Window::Current().Compositor();

        co_await winrt::resume_background();

        winrt::Windows::UI::Composition::CompositionGraphicsDevice
            compositionGraphicsDevice = winrt::Microsoft::Graphics::Canvas::UI::
            Composition::CanvasComposition::CreateCompositionGraphicsDevice(
                compositor, _canvasDevice);
        _surface = compositionGraphicsDevice.CreateDrawingSurface(
            //{ (float)_canvasWidth, (float)_canvasHeight},
            { 0,0 },
            winrt::Windows::Graphics::DirectX::DirectXPixelFormat::
            B8G8R8A8UIntNormalized,
            winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied);

        winrt::Windows::UI::Composition::CompositionSurfaceBrush brush =
            compositor.CreateSurfaceBrush(_surface);
        brush.HorizontalAlignmentRatio(.5);
        brush.VerticalAlignmentRatio(.5);
        brush.Stretch(winrt::Windows::UI::Composition::CompositionStretch::Uniform);

        winrt::Windows::UI::Composition::SpriteVisual visual =
            compositor.CreateSpriteVisual();
        visual.Brush(brush);
        visual.RelativeSizeAdjustment(
            winrt::Windows::Foundation::Numerics::float2::one());

        co_await winrt::resume_foreground(_uiThread);

        winrt::Windows::UI::Xaml::Hosting::ElementCompositionPreview::
            SetElementChildVisual(canvas, visual);
    }

    void VideoRender::OnFrame(const webrtc::VideoFrame& frame) {
        rtc::scoped_refptr<webrtc::I420BufferInterface> buffer(
            frame.video_frame_buffer()->ToI420());

        webrtc::VideoRotation rotation = frame.rotation();
        if (rotation != webrtc::kVideoRotation_0) {
            buffer = webrtc::I420Buffer::Rotate(*buffer, rotation);
        }

        int32_t width = buffer->width();
        int32_t height = buffer->height();


        if (_canvasSize < (width * height)) {
            _desiredWidth = width;
            _desiiredHeight = height;
        }

        if (!_i420_buffer_out.get() ||
            _i420_buffer_out->height() * _i420_buffer_out->width() < _desiredWidth * _desiiredHeight) {
            _i420_buffer_out = webrtc::I420Buffer::Create(_desiredWidth, _desiiredHeight);
        }

        //Hank: try to scale to match the size of the canvas, the desired display size
        _i420_buffer_out->ScaleFrom(*frame.video_frame_buffer()->ToI420());
        ::webrtc::VideoFrame::Builder scale_frame_builder =
            ::webrtc::VideoFrame::Builder()
            .set_video_frame_buffer(_i420_buffer_out)
            .set_rotation(::webrtc::kVideoRotation_0)
            .set_timestamp_us(frame.timestamp_us());

        if (frame.has_update_rect()) {
            ::webrtc::VideoFrame::UpdateRect new_rect =
                frame.update_rect().ScaleWithFrame(
                    frame.width(), frame.height(), 0, 0, frame.width(),
                    frame.height(), _desiredWidth, _desiiredHeight);
            scale_frame_builder.set_update_rect(new_rect);
        }

        size_t bits = 32;
        size_t size = _desiredWidth * _desiiredHeight * (bits >> 3);

        std::unique_ptr<uint8_t[]> data(new uint8_t[size]);
        libyuv::I420ToARGB(_i420_buffer_out->DataY(), _i420_buffer_out->StrideY(), _i420_buffer_out->DataU(),
            _i420_buffer_out->StrideU(), _i420_buffer_out->DataV(), _i420_buffer_out->StrideV(),
            data.get(), _desiredWidth * bits / 8, _desiredWidth, _desiiredHeight);

        PaintFrameAsync(std::move(data), size, _desiredWidth, _desiiredHeight);
    }

    winrt::Windows::Foundation::IAsyncAction VideoRender::PaintFrameAsync(
        std::unique_ptr<uint8_t[]> data, size_t length, int32_t width,
        int32_t height) {
        co_await winrt::resume_background();

        auto raw = data.get();
        auto view = winrt::array_view<uint8_t const>(raw, raw + length);
        auto bitmap =
            winrt::Microsoft::Graphics::Canvas::CanvasBitmap::CreateFromBytes(
                _canvasDevice, view, width, height,
                winrt::Windows::Graphics::DirectX::DirectXPixelFormat::
                B8G8R8A8UIntNormalized);

        if (_surface.Size() != bitmap.Size())
            winrt::Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::
            Resize(_surface, bitmap.Size());

        co_await winrt::resume_foreground(_uiThread);

        winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession =
            winrt::Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::
            CreateDrawingSession(_surface);
        {
            drawingSession.Clear(winrt::Windows::UI::Colors::Transparent());
            drawingSession.DrawImage(bitmap);
        }
        drawingSession.Close();
    }

} //namespace WeiRtc {