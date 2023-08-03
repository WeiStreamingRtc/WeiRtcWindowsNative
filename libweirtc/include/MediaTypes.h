#pragma once
// clang-format off
#include "pch.h"
// clang-format on#pragma once

namespace WeiRtc {

	struct WEI_EXPORT MediaTypes {

		enum VideoSourceType {
			Camera,
			Desktop,
		};

		enum AudioSourceType {
			Mic,
			System,
		};
	};

} //namespace WeiRtc {