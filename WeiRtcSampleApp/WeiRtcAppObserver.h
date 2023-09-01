#pragma once
struct WeiRtcAppObserver {
    virtual void OnPeerConnectionStatus(int status) = 0;
    virtual void OnSignallingChannelStatus(int status) = 0;
};
