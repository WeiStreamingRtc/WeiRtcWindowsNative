// clang-format off
#include "pch.h"
#include "SimpleSignalling.h"
#include "SessionDescription.h"
#include "PeerConnection.h"
// clang-format on

SimpleSignalling::SimpleSignalling()
    : _channel(nullptr), _clientId(""), _peerConnection(nullptr) {}
SimpleSignalling::~SimpleSignalling() = default;

void SimpleSignalling::RegisterPeerConnection(WeiRtc::PeerConnection* peerConnection) {
    _peerConnection = peerConnection;
}

void SimpleSignalling::StartSignalling() {
    _channel->RegisterMessageCallBack(this);
    _channel->Start();
}

void SimpleSignalling::OnMessage(std::string message) {
    try {
        winrt::Windows::Data::Json::JsonObject json_object;

        auto good = winrt::Windows::Data::Json::JsonObject::TryParse(
            winrt::to_hstring(message), json_object);

        if (!good) {
            // Hank Ice
            return;
        }
        // auto msg = winrt::Windows::Data::Json::JsonObject::Parse(winrt::to_hstring(message));
        auto type = json_object.GetNamedString(L"Type");

        if (type == L"offer") {
            auto sdpString = json_object.GetNamedString(L"sdp");

            // Set remote sdp, then create answer, if not initiator
            WeiRtc::SessionDescription sdp(WeiRtc::SessionDescription::SdpType::kOffer,
                winrt::to_string(sdpString));

            _peerConnection->SetRemoteDescription(sdp);

            _peerConnection->CreateAnswer();

            return;
        }
        if (type == L"candidate") {
            auto id = json_object.GetNamedString(L"id");
            auto label = json_object.GetNamedNumber(L"label");
            auto candidate = json_object.GetNamedString(L"candidate");

            WeiRtc::IceCandidate ice_candidate(winrt::to_string(id), static_cast<int>(label), winrt::to_string(candidate));

            bool ok = _peerConnection->AddIceCandidate(ice_candidate);
            assert(ok);

            return;
        }
        if (type == L"Server") {
            auto id = json_object.GetNamedString(L"Id");
            _clientId = winrt::to_string(id);

            return;
        }

    }
    catch (...) {
        try {
            auto j_array = winrt::Windows::Data::Json::JsonArray::Parse(
                winrt::to_hstring(message));
            auto n = j_array.Size();
        }
        catch (...) {
        }
    }
}

void SimpleSignalling::OnChannelOpen() {
    winrt::Windows::Data::Json::JsonObject j_msg;

    auto j_type = winrt::Windows::Data::Json::JsonValue::CreateStringValue(winrt::to_hstring("ClientHello"));
   // auto j_type = winrt::Windows::Data::Json::JsonValue::CreateStringValue(winrt::to_hstring("AgentHello"));

    j_msg.Insert(L"Type", j_type);

    auto j_val = winrt::Windows::Data::Json::JsonValue::CreateStringValue(
        winrt::to_hstring("Client Hello"));

    j_msg.Insert(L"Message", j_val);

    _channel->Send((winrt::to_string(j_msg.ToString())));

}
void SimpleSignalling::SendMessage(std::string message) {
    _channel->Send(message);
}

void SimpleSignalling::SendCandidate(std::string candidate) {
    winrt::Windows::Data::Json::JsonObject j_ice;

    auto j_val = winrt::Windows::Data::Json::JsonValue::CreateStringValue(
        winrt::to_hstring(candidate));
    auto j_type =
        winrt::Windows::Data::Json::JsonValue::CreateStringValue(L"candidate");
    auto j_lable = winrt::Windows::Data::Json::JsonValue::CreateNumberValue(0);
    auto j_id = winrt::Windows::Data::Json::JsonValue::CreateStringValue(L"0");

    j_ice.Insert(L"Type", j_type);
    j_ice.Insert(L"label", j_lable);
    j_ice.Insert(L"id", j_id);
    j_ice.Insert(L"candidate", j_val);

    _channel->Send((winrt::to_string(j_ice.ToString())));
}

void SimpleSignalling::SetSignallingChannel(SignallingChannel* channel) {
    this->_channel = channel;
}

void SimpleSignalling::CallSupport(winrt::hstring msg) {
    
    winrt::Windows::Data::Json::JsonObject j_msg;

    auto j_type = winrt::Windows::Data::Json::JsonValue::CreateStringValue(
        winrt::to_hstring("Request"));

    j_msg.Insert(L"Type", j_type);

    auto j_val = winrt::Windows::Data::Json::JsonValue::CreateStringValue(msg);

    j_msg.Insert(L"SupportMessage", j_val);

    auto j_id = winrt::Windows::Data::Json::JsonValue::CreateStringValue(winrt::to_hstring(_clientId));

    j_msg.Insert(L"Src", j_id);

    _channel->Send((winrt::to_string(j_msg.ToString())));
}
std::string SimpleSignalling::GetClientId() {
    return _clientId;
}
