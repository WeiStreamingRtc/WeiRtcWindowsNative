#pragma once

#include "WeiRtcApp.h"
#include "WeiRtcCall.g.h"

//using namespace WeiStreaming::WeiRtc::Native;


namespace winrt::WeiRtcSampleApp::implementation {
    struct WeiRtcCall : WeiRtcCallT<WeiRtcCall>, public WeiRtcAppObserver {
        //WeiRtcApp clazz;

        WeiRtcCall();

        void Start(hstring msg);

        void SupportRequest_Click(Windows::Foundation::IInspectable const& sender,
            Windows::UI::Xaml::RoutedEventArgs const& e);

        void OnPeerConnectionStatus(int status) override;
        void OnSignallingChannelStatus(int status) override;

    private:
        Windows::UI::Xaml::Controls::AppBarButton _hangupBtn;

    };
}  // namespace
   // winrt::WeiStreaming::WeiRtc::Native::implementation

namespace winrt::WeiRtcSampleApp::
    factory_implementation {
    struct WeiRtcCall
        : WeiRtcCallT<WeiRtcCall, implementation::WeiRtcCall> {};
}  // namespace
   // winrt::WeiStreaming::WeiRtc::Native::factory_implementation
