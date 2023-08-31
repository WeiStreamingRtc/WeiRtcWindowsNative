// clang-format off
#include "pch.h"
#include "CallStatusObserver.h"
#include "CallStatusObserver.g.cpp"

namespace winrt::WeiRtcSampleApp::implementation {

	winrt::event_token CallStatusObserver::OnCallStatus(WeiRtcSampleApp::CallStatusDelegate const& handler) {
		return _on_call_status_event.add(handler);
	}
	void CallStatusObserver::OnCallStatus(winrt::event_token const& token) noexcept {
		_on_call_status_event.remove(token);
	}

	void CallStatusObserver::OnCallStatus(int status) {
		_on_call_status_event(status);
	}


} //namespace winrt::WeiRtcSampleApp::implementation {