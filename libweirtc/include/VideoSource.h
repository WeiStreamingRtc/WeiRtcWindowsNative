#pragma once

// clang-format off
#include "pch.h"
#include "api//video/video_frame.h"
#include "api/video/video_source_interface.h"
#include "media/base/video_broadcaster.h"
#include "media/base/video_adapter.h"
#include "rtc_base/critical_section.h"
// clang-format on

namespace WeiRtc {

    class VideoSource : public rtc::VideoSourceInterface<webrtc::VideoFrame> {
        class FramePreprocessor {
        public:
            virtual ~FramePreprocessor() = default;

            virtual ::webrtc::VideoFrame Preprocess(
                const ::webrtc::VideoFrame& frame) = 0;
        };

    public:
        ~VideoSource() = default;

        void AddOrUpdateSink(rtc::VideoSinkInterface<::webrtc::VideoFrame>* sink,
            const rtc::VideoSinkWants& wants) override;
        void RemoveSink(
            rtc::VideoSinkInterface<::webrtc::VideoFrame>* sink) override;
        void SetFramePreprocessor(std::unique_ptr<FramePreprocessor> preprocessor) {
            rtc::CritScope crit(&_lock);
            preprocessor_ = std::move(preprocessor);
        }

    protected:
        void OnFrame(const ::webrtc::VideoFrame& frame);
        rtc::VideoSinkWants GetSinkWants();

    private:
        void UpdateVideoAdapter();
        ::webrtc::VideoFrame MaybePreprocess(const ::webrtc::VideoFrame& frame);

        rtc::CriticalSection _lock;
        std::unique_ptr<FramePreprocessor> preprocessor_ RTC_GUARDED_BY(_lock);
        rtc::VideoBroadcaster _broadcaster;
        cricket::VideoAdapter _video_adapter;
    };

} //namespace WeiRtc {