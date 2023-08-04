#pragma once
// clang-format off
#include "pch.h"
#include "SessionDescription.h"
#include "IceCandidate.h"
#include "PeerConnectionStates.h"
// clang-format on#pragma once
// 

namespace WeiRtc {
    //Client code need to implement this interface
    struct WEI_EXPORT PeerConnectionEventHandler
    {
        virtual void OnSdpCreated(const SessionDescription& sdp) = 0;
        virtual void OnSdpCreationFailed(const char* msg) = 0;

        virtual void OnSdpSetSuccess() = 0;
        virtual void OnSdpSetFailure(const char* msg) = 0;

        virtual void OnSignallingStateChanged(int state) = 0;
        virtual void OnStateChanged(int state) = 0;

        virtual void OnAddIceCandidateSuccess() = 0;
        virtual void OnAddIceCandidateFailure(const char* msg) = 0;

        virtual void OnIceCandidateStateChanged(int state) = 0;
        virtual void OnIceCandidateGatheringStateChanged(int state) = 0;

        virtual void OnIceCandidateGenerated(const IceCandidate& cnd) = 0;

        virtual void OnStandardizedIceConnectionChange(PeerConnectionStates::IceConnectionState state) = 0;
        virtual void OnConnectionChange(PeerConnectionStates::PeerConnectionState state) = 0;
    };
} //namespace WeiRtc {