// clang-format off
#include "pch.h"
#include "WeiRtcCall.h"
#include "WeiRtcCall.g.cpp"
// clang-format on

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

namespace winrt::WeiRtcSampleApp::implementation {
    WeiRtcCall::WeiRtcCall() {
        InitializeComponent();
        //clazz = WeiRtcApp();
    }

    void WeiRtcCall::Start(hstring msg) {

    }

    void WeiRtcCall::SupportRequest_Click(
        Windows::Foundation::IInspectable const& /*sender*/,
        Windows::UI::Xaml::RoutedEventArgs const& /*e*/) {

        Frame rootFrame{ nullptr };
        auto content = Window::Current().Content();
        if (content) {
            rootFrame = content.try_as<Frame>();
        }
        rootFrame.Navigate(xaml_typename<WeiRtcSampleApp::MainPage>(),
            box_value(L"call"));

    }

    void WeiRtcCall::OnPeerConnectionStatus(int status)
    {

        if (status == 1) {

        }
        else {
        }

    }
    void WeiRtcCall::OnSignallingChannelStatus(int status)
    {
        if (status == 1) {
        }
        else {
        }
    }


}  // namespace winrt::WeiRtcSampleApp::implementation
   // winrt::WeiRtcSampleApp::::implementation
