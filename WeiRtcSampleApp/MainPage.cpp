// clang-format off
#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <Winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
// clang-format on

using namespace winrt;
using namespace Windows::Security::Cryptography;
using namespace Windows::UI::Xaml;

#define IPV4_ADDRESS_LENGTH 16

namespace winrt::WeiRtcSampleApp::implementation {
MainPage::MainPage() {
    InitializeComponent();

    //auto room = to_hstring(CryptographicBuffer::GenerateRandomNumber());

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
    }

    // Retrieve adapter addresses
    ULONG bufferSize = 0;
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    DWORD result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, nullptr, &bufferSize);
    if (result == ERROR_BUFFER_OVERFLOW) {
        pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(new BYTE[bufferSize]);
        result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &bufferSize);
    }

    // Find WiFi adapter
    PIP_ADAPTER_ADDRESSES pAdapter = pAddresses;
    char ipAddress[IPV4_ADDRESS_LENGTH];

    while (pAdapter != nullptr) {
        if (pAdapter->IfType == IF_TYPE_IEEE80211) {
            // WiFi adapter found
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAdapter->FirstUnicastAddress;
            while (pUnicast != nullptr) {
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                    // IPv4 address found                    
                    struct sockaddr_in* pSockAddr = reinterpret_cast<struct sockaddr_in*>(pUnicast->Address.lpSockaddr);
                    strcpy_s(ipAddress, IPV4_ADDRESS_LENGTH, inet_ntoa(pSockAddr->sin_addr));
                    std::cout << "WiFi Adapter IP Address: " << ipAddress << std::endl;
                    if (strncmp(ipAddress, "192.168", 7) == 0) {
                        std::cout << "WiFi Adapter IP Address (starting with 192.168): " << ipAddress << std::endl;
                        break;
                    }
                }
                pUnicast = pUnicast->Next;
            }
        }
        pAdapter = pAdapter->Next;
    }

    // Clean up
    delete[] reinterpret_cast<BYTE*>(pAddresses);
    WSACleanup();
    
    Control().Start(to_hstring(ipAddress));
}
}  // namespace winrt::WeiRtcSampleApp::implementation
