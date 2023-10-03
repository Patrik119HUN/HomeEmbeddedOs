#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <STM32FreeRTOS.h>
#include <SdFat.h>
#include <sys/log.h>

#include "../../kernel/network_manager/network_manager.h"
#include "wifid.h"
void convertFromJson(JsonVariantConst src, IPAddress& dst) { dst.fromString(src.as<const char*>()); }
void network_handler_deamon(void*) {
    ExFile file, dir;
    IPAddress ip, dns, gateway, subnet;
    StaticJsonDocument<512> m_json;
    char fname[20];
    if (!dir.open("network")) ERROR("Failed to open SD Card folder");
    while (file.openNext(&dir, O_RDONLY)) {
        if (!file.isDir()) file.getName7(fname, 20);
        DeserializationError error = deserializeJson(m_json, file);
        if (error) {
            ERROR("deserializeJson() failed: %d", error.c_str());
            return;
        }
        file.close();
        bool m_static = m_json["static"];
        if (m_static) {
            ip = m_json["address"]["ip"].as<IPAddress>();
            dns = m_json["address"]["dns"].as<IPAddress>();
            gateway = m_json["address"]["gateway"].as<IPAddress>();
            subnet = m_json["address"]["subnet"].as<IPAddress>();
            AddressConfig conf{ip, dns, gateway, subnet};
        }
        bool keep_alive = m_json["keep-alive"];
        string m_sinterface = m_json["adapterType"];
        std::transform(m_sinterface.begin(), m_sinterface.end(), m_sinterface.begin(), ::tolower);
        INFO("Its an %s adapter, named: %s", m_sinterface.c_str(), fname);
        if (m_sinterface == "wifi") {
            string ssid = m_json["connection"]["ssid"];
            string pass = m_json["connection"]["password"];

            WiFiAdapter* wptr = new WiFiAdapter(string(fname), ssid, pass);
            networkManager.add_adapter(wptr);
            processManager.start_process(string(fname), wifi_deamon, static_cast<void*>(wptr));
        }
        if (m_sinterface == "ethernet") {
            EthernetAdapter* ethptr = new EthernetAdapter(string(fname));
            networkManager.add_adapter(ethptr);
            processManager.start_process(string(fname), ethernet_deamon, static_cast<void*>(ethptr));
        }
        taskYIELD();
    }
    auto adapter_vector = networkManager.get_adapters();
    INetworkAdapter* currentAdaper = networkManager.get_adapter(adapter_vector.at(0));
    while (true) {
        for (auto adapter_name : adapter_vector) {
            INetworkAdapter* adapter = networkManager.get_adapter(adapter_name);

            if (adapter == currentAdaper) continue;
            if (adapter->getStatus() != connectionState::CONNECTED) continue;
            if (currentAdaper->getStatus() != connectionState::CONNECTED) {
                INFO("Changing network to: %s", adapter->getName().c_str());
                currentAdaper = adapter;
                networkManager.set_stack(adapter_name);
                continue;
            }
            if (adapter->get_priority() >= currentAdaper->get_priority()) continue;
            INFO("Changig network to: %s", adapter->getName().c_str());
            currentAdaper = adapter;
            networkManager.set_stack(adapter_name);
        }
        taskYIELD();
    }
}

/*

Mikor kell váltani?

    Adapter neve    Rang    Státusz             Jelenlegi

    Wi-Fi           1       CONNECTED
    ETHERNET        0       CONNECTED       =   Ethernet

    Wi-Fi           1       DISCONNECTED
    ETHERNET        0       CONNECTED       =   Ethernet

    Wi-Fi           1       CONNECTED
    ETHERNET        0       DISCONNECTED    =   Wi-Fi
*/