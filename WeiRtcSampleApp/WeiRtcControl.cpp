// clang-format off
#include "pch.h"
#include "WeiRtcControl.h"
#include "WeiRtcControl.g.cpp"
// clang-format on

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::WeiRtcSampleApp::implementation {
WeiRtcControl::WeiRtcControl() {
    InitializeComponent();
    clazz = winrt::WeiRtcSampleApp::WeiRtcApp();
}

hstring WeiRtcControl::IpAddress() { return clazz.Room(); }

void WeiRtcControl::IpAddress(hstring value) {}

void WeiRtcControl::Start(hstring value) { clazz.Init(Canvas(), pipCanvas(), screenCaptureCanvas(), value); }

void WeiRtcControl::WeiRTCButton_Click(
    Windows::Foundation::IInspectable const& /*sender*/,
    Windows::UI::Xaml::RoutedEventArgs const& /*e*/) {}


void WeiRtcControl::ScreenShare_Click(
    Windows::Foundation::IInspectable const& /*sender*/,
    Windows::UI::Xaml::RoutedEventArgs const& /*e*/) {
    
    clazz.StartDesktopCaptuer();

}

}// namespace
   // winrt::WeiRtcSampleApp::::implementation
