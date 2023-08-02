#pragma once

#include "WeiRtcApp.g.h"

namespace winrt::WeiRtcSampleApp::implementation {
struct WeiRtcApp : WeiRtcAppT<WeiRtcApp> {
    struct WebRtcSample *_sample = nullptr;

    WeiRtcApp();
    ~WeiRtcApp();

    hstring _room;
    hstring Room() const;

    Windows::Foundation::IAsyncAction WeiRtcApp::Init(
        Windows::UI::Xaml::UIElement canvas, Windows::UI::Xaml::UIElement pipCanvas, Windows::UI::Xaml::UIElement screenPipCanvas, hstring room);
    Windows::Foundation::IAsyncAction StartDesktopCaptuer();

private:
    Windows::UI::Xaml::UIElement* _screenPipCanvas;

};
}  // namespace
   // winrt::WeiRtcSampleApp::implementation 

namespace winrt::WeiRtcSampleApp::factory_implementation {
struct WeiRtcApp : WeiRtcAppT<WeiRtcApp, implementation::WeiRtcApp> {};
}  // namespace
   // winrt::WeiRtcSampleApp::factory_implementation
