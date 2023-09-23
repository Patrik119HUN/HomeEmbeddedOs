#pragma once

#include "const.h"
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <network_adapter_interface.h>
using std::string, std::pair;

class WiFiAdapter : public INetworkAdapter {
  public:
    WiFiAdapter(
        const string& name, const string& ssid, const string& pass, bool const keep_alive = true
    )
        : INetworkAdapter{name, 1, keep_alive, adapterType::WIFI}, _ssid{ssid}, _pass{pass} {}

    virtual Client* get_client() override { return &_wifi_client; }
    virtual UDP* get_udp() override { return &_wifi_udp; }
    pair<string, string> getConnectionData() { return std::make_pair(_ssid, _pass); }
    int begin() override {
        WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
        if (WiFi.status() == WL_NO_MODULE) {
            return 1;
        }
        return 0;
    }


  private:
    string _ssid;
    string _pass;

    WiFiUDP _wifi_udp;
    WiFiClient _wifi_client;
};
