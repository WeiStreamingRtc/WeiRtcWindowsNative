// clang-format off
#include "pch.h"
#include "webSocketSignalling\SimpleWebSocketSignaling.h"
#include "SessionDescription.h"
#include "PeerConnection.h"
// clang-format on

SimpleWebSocketSignaling::SimpleWebSocketSignaling(std::string url)
    : _signallingChannel(url), _peerConnection(nullptr) {}
SimpleWebSocketSignaling::~SimpleWebSocketSignaling() = default;

void SimpleWebSocketSignaling::RegisterPeerConnection(WeiRtc::PeerConnection* peerConnection) {
    _peerConnection = peerConnection;
}

void SimpleWebSocketSignaling::StartSignalling() {
    _signallingChannel.RegisterMessageCallBack(this);
    _signallingChannel.Start();
}

void SimpleWebSocketSignaling::OnWebSocketMessage(std::string message) {
 
    try {

        winrt::Windows::Data::Json::JsonObject json_object;

        auto good = winrt::Windows::Data::Json::JsonObject::TryParse(
            winrt::to_hstring(message), json_object);

        if (!good) {
            // Hank Ice
            return;
        }
        // auto msg = winrt::Windows::Data::Json::JsonObject::Parse(winrt::to_hstring(message));
        auto type = json_object.GetNamedString(L"type");

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

    } catch (...) {
        try {
            auto j_array = winrt::Windows::Data::Json::JsonArray::Parse(
                winrt::to_hstring(message));
            auto n = j_array.Size();
        } catch (...) {
        }
    }
}

void SimpleWebSocketSignaling::SendMessage(std::string message) {
    _signallingChannel.Send(message);
}

void SimpleWebSocketSignaling::SendCandidate(std::string candidate) {
    winrt::Windows::Data::Json::JsonObject j_ice;

    auto j_val = winrt::Windows::Data::Json::JsonValue::CreateStringValue(
        winrt::to_hstring(candidate));
    auto j_type =
        winrt::Windows::Data::Json::JsonValue::CreateStringValue(L"candidate");
    auto j_lable = winrt::Windows::Data::Json::JsonValue::CreateNumberValue(0);
    auto j_id = winrt::Windows::Data::Json::JsonValue::CreateStringValue(L"0");

    j_ice.Insert(L"type", j_type);
    j_ice.Insert(L"label", j_lable);
    j_ice.Insert(L"id", j_id);
    j_ice.Insert(L"candidate", j_val);

    _signallingChannel.Send((winrt::to_string(j_ice.ToString())));
}