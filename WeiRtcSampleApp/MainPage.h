#pragma once

#include "MainPage.g.h"

namespace winrt::WeiRtcSampleApp::implementation {
struct MainPage : MainPageT<MainPage> {
    MainPage();
};
}  // namespace winrt::WeiRtcSampleApp::implementation

namespace winrt::WeiRtcSampleApp::factory_implementation {
struct MainPage : MainPageT<MainPage, implementation::MainPage> {};
}  // namespace winrt::WeiRtcSampleApp::factory_implementation
