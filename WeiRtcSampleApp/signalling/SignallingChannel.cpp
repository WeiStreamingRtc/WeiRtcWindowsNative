// clang-format off
#include "pch.h"
#include "SignallingChannel.h"
// clang-format on

int SignallingChannel::Start()
{
	return 0;
}
void SignallingChannel::RegisterMessageCallBack(SignallingChannelMessageHandler* handler) {
	_messageHandler = handler;
}

void SignallingChannel::Send(std::string message) {

}