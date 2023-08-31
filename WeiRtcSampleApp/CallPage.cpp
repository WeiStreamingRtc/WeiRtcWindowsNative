#include "pch.h"
#include "CallPage.h"
#if __has_include("CallPage.g.cpp")
#include "CallPage.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::WeiRtcSampleApp::implementation
{
    CallPage::CallPage() {
        InitializeComponent();
        Control().Start(L"");
    }
    
}
