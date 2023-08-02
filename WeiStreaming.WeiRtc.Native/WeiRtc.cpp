// clang-format off
#include "pch.h"
#include "WeiRtc.h"
// clang-format on

namespace WeiRtc {

bool InitializeWeiRtc() {
	return rtc::InitializeSSL();
}
bool CleanupWeiRtc() {
	return rtc::CleanupSSL();
}

}  // namespace WeiRtc