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
    clazz = WeiRtcApp();
}

hstring WeiRtcControl::IpAddress() { return clazz.Room(); }

void WeiRtcControl::IpAddress(hstring value) {}

void WeiRtcControl::Start(hstring value) {
    clazz.Init(Canvas(), pipCanvas(), screenCaptureCanvas(), value);
    clazz.RegisterAppObserver(this);

    _serverBtn = ServerButton();
    _serverBtn.Label(value);

}

void WeiRtcControl::WeiRTCButton_Click(
    Windows::Foundation::IInspectable const& /*sender*/,
    Windows::UI::Xaml::RoutedEventArgs const& /*e*/) {}

void WeiRtcControl::ScreenShare_Click(
    Windows::Foundation::IInspectable const& /*sender*/,
    Windows::UI::Xaml::RoutedEventArgs const& /*e*/) {
    clazz.StartDesktopCaptuer();
}

void WeiRtcControl::SupportRequest_Click(
    Windows::Foundation::IInspectable const& /*sender*/,
    Windows::UI::Xaml::RoutedEventArgs const& /*e*/) {

    _textBox = SupportDesc();
    _requestBtn = SupportRequest();
    _titleBlock = SupportTitle();


    _ring = ProgressRing();

    winrt::hstring msg = _textBox.Text();
    _textBox.Text(L"");
    clazz.CallSupport(msg);
    _ring.IsActive(true);
    _requestBtn.IsEnabled(false);


    _textBox.Visibility(Windows::UI::Xaml::Visibility::Collapsed);
    _titleBlock.Visibility(Windows::UI::Xaml::Visibility::Collapsed);
}

void WeiRtcControl::OnPeerConnectionStatus(int status)
{

    if (status == 1) {
        _ring.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler([this]
                {
                _ring.IsActive(false);
                }));

    }
    else if (status == 3) {
        _ring.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler([this]
            {
                _ring.IsActive(true);
            }));
        _titleBlock.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler([this]
            {
                _titleBlock.Visibility(Windows::UI::Xaml::Visibility::Visible);
                _titleBlock.Text(L"Remote user hangup...");
            }));           
    }
    else {
        _ring.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler([this]
            {
                _ring.IsActive(true);
            }));
        _titleBlock.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler([this]
            {
                _titleBlock.Visibility(Windows::UI::Xaml::Visibility::Visible);
                _titleBlock.Text(L"Call has been dropped.........");
            }));

        Sleep(50000);
        _requestBtn.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler([this]
            {                                
                auto win = Window::Current();
                win.Close();
            }));
    }

}
void WeiRtcControl::OnSignallingChannelStatus(int status)
{
    if (status == 1) {
        _requestBtn.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler([this]
            {
                _requestBtn.IsEnabled(true); //status is not captured 

            }));
    }
    else {
        _requestBtn.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler([this]
            {
                _requestBtn.IsEnabled(false);

            }));
    }
}


}  // namespace winrt::WeiRtcSampleApp::implementation
   // winrt::WeiRtcSampleApp::::implementation
