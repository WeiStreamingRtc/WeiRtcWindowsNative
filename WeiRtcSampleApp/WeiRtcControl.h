#pragma once

#include "WeiRtcControl.g.h"

//using namespace WeiStreaming::WeiRtc::Native;


namespace winrt::WeiRtcSampleApp::implementation {
struct WeiRtcControl : WeiRtcControlT<WeiRtcControl> {
    winrt::WeiRtcSampleApp::WeiRtcApp clazz{nullptr};

    WeiRtcControl();
    
    hstring IpAddress();

    void IpAddress(hstring value);

    void Start(hstring value);

    void WeiRTCButton_Click(Windows::Foundation::IInspectable const &sender,
                            Windows::UI::Xaml::RoutedEventArgs const &e);

    void ScreenShare_Click(Windows::Foundation::IInspectable const& sender,
        Windows::UI::Xaml::RoutedEventArgs const& e);

};
}  // namespace
   // winrt::WeiStreaming::WeiRtc::Native::implementation

namespace winrt::WeiRtcSampleApp::
    factory_implementation {
struct WeiRtcControl
    : WeiRtcControlT<WeiRtcControl, implementation::WeiRtcControl> {};
}  // namespace
   // winrt::WeiStreaming::WeiRtc::Native::factory_implementation
