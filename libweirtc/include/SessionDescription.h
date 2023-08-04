#pragma once

// clang-format off
#include "pch.h"
// clang-format on#pragma once

namespace WeiRtc {

	class WEI_EXPORT SessionDescription
	{
	public:
		enum class SdpType {
			kOffer,
			kPrAnswer,
			kAnswer,
			kRollback
		};
		SessionDescription(SdpType type, std::string sdpString);
		~SessionDescription();

		webrtc::SessionDescriptionInterface* GetIntance() const;
		std::string GetSdpAsString() const;
	private:
		std::unique_ptr<webrtc::SessionDescriptionInterface> _instance;
		std::string _sdpString;
	};

} //namespace WeiRtc {