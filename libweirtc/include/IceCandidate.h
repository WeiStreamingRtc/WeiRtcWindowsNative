#pragma once

// clang-format off
#include "pch.h"
// clang-format on#pragma once

namespace WeiRtc {

	class WEI_EXPORT IceCandidate
	{
	public:
		IceCandidate(const std::string& sdp_mid, int sdp_mline_index, const std::string& candidate);
		IceCandidate(const std::string& candidate);
		~IceCandidate() = default;

		std::string GetID() const;
		int GetIndex() const;
		std::string GetCandidate() const;

	private:
		std::string _mid;
		int _midIndex;
		std::string _candidate;
	};

} //namespace WeiRtc {