#pragma once

// clang-format off
#include "pch.h"
// clang-format on
namespace WeiRtc {
	struct WEI_EXPORT VideoCapturer {
		virtual bool StartCapture() = 0;
		virtual void StopCapture() = 0;
		virtual void Dispose() = 0;
		virtual bool IsDesktopCapturer() = 0;
		virtual int GetVideoWidth() = 0;
		virtual int GetVideoHeight() = 0;
	};
}// namespace WeiRtc {