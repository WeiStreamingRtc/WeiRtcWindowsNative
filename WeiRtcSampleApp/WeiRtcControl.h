#pragma once

#include "WeiRtcApp.h"
#include "WeiRtcAppObserver.h"
#include "WeiRtcControl.g.h"

//using namespace WeiStreaming::WeiRtc::Native;


namespace winrt::WeiRtcSampleApp::implementation {
struct WeiRtcControl : WeiRtcControlT<WeiRtcControl>, public WeiRtcAppObserver {
    WeiRtcApp clazz;

    WeiRtcControl();
    
    hstring IpAddress();

    void IpAddress(hstring value);

    void Start(hstring value);

    void WeiRTCButton_Click(Windows::Foundation::IInspectable const &sender,
                            Windows::UI::Xaml::RoutedEventArgs const &e);

    void ScreenShare_Click(Windows::Foundation::IInspectable const& sender,
        Windows::UI::Xaml::RoutedEventArgs const& e);

    void SupportRequest_Click(Windows::Foundation::IInspectable const& sender,
        Windows::UI::Xaml::RoutedEventArgs const& e);

    void OnPeerConnectionStatus(int status) override;
    void OnSignallingChannelStatus(int status) override;

private:
    Windows::UI::Xaml::Controls::ProgressRing _ring;
    Windows::UI::Xaml::Controls::TextBox _textBox;
    Windows::UI::Xaml::Controls::AppBarButton _requestBtn;
    Windows::UI::Xaml::Controls::TextBlock _titleBlock;
    Windows::UI::Xaml::Controls::AppBarButton _serverBtn;

};
}  // namespace
   // winrt::WeiStreaming::WeiRtc::Native::implementation

namespace winrt::WeiRtcSampleApp::
    factory_implementation {
struct WeiRtcControl
    : WeiRtcControlT<WeiRtcControl, implementation::WeiRtcControl> {};
}  // namespace
   // winrt::WeiStreaming::WeiRtc::Native::factory_implementation
