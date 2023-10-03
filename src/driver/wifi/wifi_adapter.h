#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiEspAT.h>
#include <WiFiUdp.h>
#include <network_adapter_interface.h>
#include <types.h>

#include <iomanip>
#include <ranges>
#include <sstream>

#include "const.h"
using std::string, std::pair;
class WiFiAdapter : public INetworkAdapter {
   public:
    WiFiAdapter(const string& name, const string& ssid, const string& pass, bool const keep_alive = true, OptAddress conf = nullopt)
        : INetworkAdapter{name, 1, keep_alive, conf, adapterType::WIFI}, _ssid{ssid}, _pass{pass} {
        if (!m_static) return;
        Serial.printf("Network Adapter:\n");
        Serial.printf("\tIP:\t%s\n", ip_to_string(m_ip));
        Serial.printf("\tDNS:\t%s\n", ip_to_string(m_dns));
        Serial.printf("\tGatway:\t%s\n", ip_to_string(m_gateway));
        Serial.printf("\tSubnet:\t%s\n", ip_to_string(m_subnet));
    }
    WiFiAdapter(const string& name, bool const keep_alive = true, OptAddress conf = nullopt) : WiFiAdapter{name, "", "", keep_alive, conf} {}

    Client* get_client() { return &_wifi_client; }
    UDP* get_udp() { return &_wifi_udp; }
    StringPair getConnectionData() { return std::make_pair(_ssid, _pass); }
    void listNetworks() {
        // scan for nearby networks:
        Serial.println("** Scan Networks **");
        int numSsid = WiFi.scanNetworks();
        if (numSsid == -1) {
            Serial.println("Couldn't get a WiFi connection");
            while (true)
                ;
        }
        // print the list of networks seen:
        Serial.print("number of available networks: ");
        Serial.println(numSsid);
        // print the network number and name for each network found:
        for (int thisNet = 0; thisNet < numSsid; thisNet++)
            Serial.printf("%d) Signal: %d dBm \tChannel: %d \t\t SSID: %s\n", thisNet + 1, WiFi.RSSI(thisNet), WiFi.channel(thisNet), WiFi.SSID(thisNet));
    }
    std::string printMacAddress(byte mac[]) {
        std::stringstream buffer;
        for (int i = 5; i >= 0; i--) {
            buffer << std::setw(2) << std::setfill('0') << std::hex << (int)mac[i] << ":";
        }
        std::string ret = buffer.str();
        ret.pop_back();
        return ret;
    }
    int begin() {
        if (WiFi.status() == WL_NO_MODULE) {
            Serial.print("FAILED");
            return 1;
        }
        byte mac[6];
        WiFi.macAddress(mac);
        Serial.printf("MAC: %s \n", printMacAddress(mac).c_str());
        Serial.println("Scanning available networks...");
        listNetworks();
        return 0;
    }

   private:
    string _ssid;
    string _pass;

    WiFiUDP _wifi_udp;
    WiFiClient _wifi_client;
};
