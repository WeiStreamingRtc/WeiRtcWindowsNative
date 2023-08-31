#pragma once

#include "CallPage.g.h"

namespace winrt::WeiRtcSampleApp::implementation {
    struct CallPage : CallPageT<CallPage> {
        CallPage();
    };
}  // namespace winrt::WeiRtcSampleApp::implementation

namespace winrt::WeiRtcSampleApp::factory_implementation {
    struct CallPage : CallPageT<CallPage, implementation::CallPage> {};
}  // namespace winrt::WeiRtcSampleApp::factory_implementation
