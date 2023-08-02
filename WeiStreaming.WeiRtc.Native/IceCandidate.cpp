// clang-format off
#include "pch.h"
#include "IceCandidate.h"
// clang-format on
namespace WeiRtc {

    IceCandidate::IceCandidate(const std::string& sdp_mid, int sdp_mline_index,
        const std::string& candidate)
        : _mid(sdp_mid), _midIndex(sdp_mline_index), _candidate(candidate) {}

    IceCandidate::IceCandidate(const std::string& candidate)
        : _mid(""), _midIndex(0), _candidate(candidate) {}

    std::string IceCandidate::GetID() const {
        return _mid;
    }

    int IceCandidate::GetIndex() const {
        return _midIndex;
    }

    std::string IceCandidate::GetCandidate() const {
        return _candidate;
    }

} //namespace WeiRtc {