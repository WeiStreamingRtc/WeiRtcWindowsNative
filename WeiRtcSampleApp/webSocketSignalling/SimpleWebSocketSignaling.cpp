// clang-format off
#include "pch.h"
#include "webSocketSignalling\SimpleWebSocketSignaling.h"
#include "SessionDescription.h"
#include "PeerConnection.h"
// clang-format on

SimpleWebSocketSignaling::SimpleWebSocketSignaling(std::string url)
    : _signallingChannel(url), SimpleSignalling() {}
SimpleWebSocketSignaling::~SimpleWebSocketSignaling() = default;

void SimpleWebSocketSignaling::StartSignalling() {
    SimpleSignalling::SetSignallingChannel(&_signallingChannel);
    SimpleSignalling::StartSignalling();
    _signallingChannel.Start();
}