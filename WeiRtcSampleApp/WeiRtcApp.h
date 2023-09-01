#pragma once
#include "WeiRtcAppObserver.h"
struct WeiRtcApp {
    struct WebRtcSample* _sample = nullptr;

    WeiRtcApp();
    ~WeiRtcApp();

    winrt::hstring _room;
    winrt::hstring Room() const;

    winrt::Windows::Foundation::IAsyncAction WeiRtcApp::Init(
        winrt::Windows::UI::Xaml::UIElement canvas, winrt::Windows::UI::Xaml::UIElement pipCanvas, winrt::Windows::UI::Xaml::UIElement screenPipCanvas, winrt::hstring room);
    winrt::Windows::Foundation::IAsyncAction StartDesktopCaptuer();

    void CallSupport(winrt::hstring msg);

    void RegisterAppObserver(WeiRtcAppObserver* observer);

private:
    winrt::Windows::UI::Xaml::UIElement* _screenPipCanvas;
    WeiRtcAppObserver* _observer;
};