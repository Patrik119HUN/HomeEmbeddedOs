#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiEspAT.h>
#include <WiFiUdp.h>
#include <network_adapter_interface.h>

#include "const.h"
using std::string, std::pair;
const char* ip_to_string(const IPAddress& address) {
    char* buf = (char*)malloc(20 * sizeof(char));
    sprintf(buf, "%u.%u.%u.%u", address[0], address[1], address[2], address[3]);
    return buf;
}
class WiFiAdapter : public INetworkAdapter {
   public:
    WiFiAdapter(const string& name, StreamFile<ExFatFile, uint64_t>* file, bool const keep_alive = true) : INetworkAdapter{name, file, 1} {
        _ssid = m_json["connection"]["ssid"].as<string>();
        _pass = m_json["connection"]["password"].as<string>();
        Serial.printf("Network Adapter:\n");
        Serial.printf("\tType:\t%s\n", m_sinterface.c_str());
        Serial.printf("\tIP:\t%s\n", ip_to_string(m_ip));
        Serial.printf("\tDNS:\t%s\n", ip_to_string(m_dns));
        Serial.printf("\tGatway:\t%s\n", ip_to_string(m_gateway));
        Serial.printf("\tSubnet:\t%s\n", ip_to_string(m_subnet));
    }
    WiFiAdapter(const string& name, StreamFile<ExFatFile, uint64_t>* file, const string& ssid, const string& pass, bool const keep_alive = true)
        : INetworkAdapter{name, file, 1, keep_alive, adapterType::WIFI}, _ssid{ssid}, _pass{pass} {}

    virtual Client* get_client() override { return &_wifi_client; }
    virtual UDP* get_udp() override { return &_wifi_udp; }
    pair<string, string> getConnectionData() { return std::make_pair(_ssid, _pass); }
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
        for (int thisNet = 0; thisNet < numSsid; thisNet++) {
            Serial.print(thisNet + 1);
            Serial.print(") ");
            Serial.print("Signal: ");
            Serial.print(WiFi.RSSI(thisNet));
            Serial.print(" dBm");
            Serial.print("\tChannel: ");
            Serial.print(WiFi.channel(thisNet));
            byte bssid[6];
            Serial.print("\tEncryption: ");
            Serial.print("\t\tSSID: ");
            Serial.println(WiFi.SSID(thisNet));
            Serial.flush();
        }
        Serial.println();
    }
    void printMacAddress(byte mac[]) {
        for (int i = 5; i >= 0; i--) {
            if (mac[i] < 16) {
                Serial.print("0");
            }
            Serial.print(mac[i], HEX);
            if (i > 0) {
                Serial.print(":");
            }
        }
        Serial.println();
    }
    int begin() override {
        //WiFi.disconnect();
        
        // if (_static) WiFi.config(ip, dns, gateway, subnet);
        if (WiFi.status() == WL_NO_MODULE) {
            Serial.print("FAILED");
            // return 1;
        }
        byte mac[6];

        WiFi.macAddress(mac);
        Serial.print("MAC: ");
        printMacAddress(mac);
        // scan for existing networks:
        Serial.println();
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
