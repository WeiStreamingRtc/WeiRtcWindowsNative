// clang-format off
#include "pch.h"
#include "VideoSource.h"
#include "api/video/i420_buffer.h"
// clang-format on

namespace WeiRtc {

    void VideoSource::OnFrame(const ::webrtc::VideoFrame& original_frame) {
        int cropped_width = 0;
        int cropped_height = 0;
        int out_width = 0;
        int out_height = 0;

        ::webrtc::VideoFrame frame = MaybePreprocess(original_frame);

        if (!_video_adapter.AdaptFrameResolution(
            frame.width(), frame.height(), frame.timestamp_us() * 1000,
            &cropped_width, &cropped_height, &out_width, &out_height)) {
            // Drop frame in order to respect frame rate constraint.

            return;
        }

        if (out_height != frame.height() || out_width != frame.width()) {
            // Video adapter has requested a down-scale. Allocate a new buffer and
            // return scaled version.
            // For simplicity, only scale here without cropping.
            ::rtc::scoped_refptr<::webrtc::I420Buffer> scaled_buffer =
                ::webrtc::I420Buffer::Create(out_width, out_height);
            scaled_buffer->ScaleFrom(*frame.video_frame_buffer()->ToI420());
            ::webrtc::VideoFrame::Builder new_frame_builder =
                ::webrtc::VideoFrame::Builder()
                .set_video_frame_buffer(scaled_buffer)
                .set_rotation(::webrtc::kVideoRotation_0)
                .set_timestamp_us(frame.timestamp_us())
                .set_id(frame.id());
            /* libwebrtc release version does not expose this api
            if (frame.has_update_rect()) {
                ::webrtc::VideoFrame::UpdateRect new_rect =
                    frame.update_rect().ScaleWithFrame(
                        frame.width(), frame.height(), 0, 0, frame.width(),
                        frame.height(), out_width, out_height);

                new_frame_builder.set_update_rect(new_rect);
            }
            */
            _broadcaster.OnFrame(new_frame_builder.build());
        }
        else {
            // No adaptations needed, just return the frame as is.
            _broadcaster.OnFrame(frame);
        }
    }

    rtc::VideoSinkWants VideoSource::GetSinkWants() {
        return _broadcaster.wants();
    }
    //Hank: when peer connection connected, this is called by libwebrtc also. this is called when connect with pip also.
    void VideoSource::AddOrUpdateSink(
        rtc::VideoSinkInterface<::webrtc::VideoFrame>* sink,
        const rtc::VideoSinkWants& wants) {
        _broadcaster.AddOrUpdateSink(sink, wants);
        UpdateVideoAdapter();
    }

    void VideoSource::RemoveSink(
        rtc::VideoSinkInterface<::webrtc::VideoFrame>* sink) {
        _broadcaster.RemoveSink(sink);
        UpdateVideoAdapter();
    }

    void VideoSource::UpdateVideoAdapter() {
        //Hank: This is where the when the peer get connected, will set the max frame rate of the adapter,
        //When initialized, the frame rate is max of int, this will set to the desired like 60fps
        //adapter will depends on the frame rate and timestamp of the frame to decice if drop a frame.
        //this will update other fields also like:  resolution_request_max_pixel_count_
        _video_adapter.OnSinkWants(_broadcaster.wants());
    }

    ::webrtc::VideoFrame VideoSource::MaybePreprocess(
        const ::webrtc::VideoFrame& frame) {
        rtc::CritScope crit(&_lock);
        if (preprocessor_ != nullptr) {
            return preprocessor_->Preprocess(frame);
        }
        else {
            return frame;
        }
    }

} //namespace WeiRtc {