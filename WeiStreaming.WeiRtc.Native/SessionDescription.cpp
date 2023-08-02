// clang-format off
#include "pch.h"
#include <stdexcept>
#include "api/jsep.h"
#include "SessionDescription.h"
// clang-format on

namespace WeiRtc {

	SessionDescription::SessionDescription(SdpType type, std::string sdpString) : _sdpString(sdpString) {

		webrtc::SdpType webrtcType;

		switch (type)
		{
		case SessionDescription::SdpType::kOffer:
			webrtcType = webrtc::SdpType::kOffer;
			break;
		case SessionDescription::SdpType::kPrAnswer:
			webrtcType = webrtc::SdpType::kPrAnswer;
			break;
		case SessionDescription::SdpType::kAnswer:
			webrtcType = webrtc::SdpType::kAnswer;
			break;
		case SessionDescription::SdpType::kRollback:
			webrtcType = webrtc::SdpType::kRollback;
			break;
		default:
			throw std::invalid_argument("Invalid SdpType");
			break;
		}

		_instance = webrtc::CreateSessionDescription(webrtcType, sdpString);
	}
	SessionDescription::~SessionDescription() {
		_instance.release();
	}

	webrtc::SessionDescriptionInterface* SessionDescription::GetIntance() const {
		return _instance.get();
	}

	std::string SessionDescription::GetSdpAsString() const {
		return _sdpString;
	}

} //namespace WeiRtc {