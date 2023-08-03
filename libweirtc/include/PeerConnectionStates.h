#pragma once
// clang-format off
#include "pch.h"
// clang-format on#pragma once

namespace WeiRtc {

    struct WEI_EXPORT PeerConnectionStates {
        // See https://w3c.github.io/webrtc-pc/#dom-rtcsignalingstate
        enum SignalingState {
            kStable,
            kHaveLocalOffer,
            kHaveLocalPrAnswer,
            kHaveRemoteOffer,
            kHaveRemotePrAnswer,
            kClosed,
        };

        // See https://w3c.github.io/webrtc-pc/#dom-rtcicegatheringstate
        enum IceGatheringState {
            kIceGatheringNew,
            kIceGatheringGathering,
            kIceGatheringComplete
        };

        // See https://w3c.github.io/webrtc-pc/#dom-rtcpeerconnectionstate
        enum class PeerConnectionState {
            kNew,
            kConnecting,
            kConnected,
            kDisconnected,
            kFailed,
            kClosed,
        };

        // See https://w3c.github.io/webrtc-pc/#dom-rtciceconnectionstate
        enum IceConnectionState {
            kIceConnectionNew,
            kIceConnectionChecking,
            kIceConnectionConnected,
            kIceConnectionCompleted,
            kIceConnectionFailed,
            kIceConnectionDisconnected,
            kIceConnectionClosed,
            kIceConnectionMax,
        };
    };

} //namespace WeiRtc {