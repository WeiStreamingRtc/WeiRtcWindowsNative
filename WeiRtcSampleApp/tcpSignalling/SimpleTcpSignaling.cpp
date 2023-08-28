// clang-format off
#include "pch.h"
#include "SimpleTcpSignaling.h"
#include "SessionDescription.h"
#include "PeerConnection.h"
// clang-format on

SimpleTcpSignaling::SimpleTcpSignaling()
    : _tcpSignallingServer(8888), SimpleSignalling() {}
SimpleTcpSignaling::~SimpleTcpSignaling() = default;

void SimpleTcpSignaling::StartSignalling() {
    SimpleSignalling::SetSignallingChannel(&_tcpSignallingServer);
    SimpleSignalling::StartSignalling();
    _tcpSignallingServer.Start();    
}