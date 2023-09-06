#pragma once

#include "MainPage.g.h"

namespace winrt::WeiRtcSampleApp::implementation {
struct MainPage : MainPageT<MainPage> {
    MainPage();
    winrt::Windows::Foundation::IAsyncAction ReadLineFromFileAsync();
    winrt::Windows::Foundation::IAsyncAction InitializeAsync();

private:
    winrt::hstring _wsUrl;
};
}  // namespace winrt::WeiRtcSampleApp::implementation

namespace winrt::WeiRtcSampleApp::factory_implementation {
struct MainPage : MainPageT<MainPage, implementation::MainPage> {};
}  // namespace winrt::WeiRtcSampleApp::factory_implementation
