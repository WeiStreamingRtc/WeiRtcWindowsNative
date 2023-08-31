#pragma once
#include "CallStatusObserver.g.h"
#include "AppCallStatusObserver.h"

namespace winrt::WeiRtcSampleApp::implementation {

	struct CallStatusObserver : CallStatusObserverT<CallStatusObserver>, public AppCallStatusObserver
	{
		CallStatusObserver() = default;

		winrt::event_token OnCallStatus(WeiRtcSampleApp::CallStatusDelegate const& handler);
		void OnCallStatus(winrt::event_token const& token) noexcept;

		void OnCallStatus(int status) override;

	private:
		winrt::event<WeiRtcSampleApp::CallStatusDelegate> _on_call_status_event;
	};
}

namespace winrt::WeiRtcSampleApp::factory_implementation {
	struct CallStatusObserver : CallStatusObserverT<CallStatusObserver, implementation::CallStatusObserver> {};
}  // namespace
   // winrt::WeiRtcSampleApp::factory_implementation