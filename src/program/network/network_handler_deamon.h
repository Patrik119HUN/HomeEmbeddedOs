#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <STM32FreeRTOS.h>
#include <SdFat.h>
#include <sys/log.h>

#include "../../kernel/network_manager/network_manager.h"
#include "wifid.h"
void convertFromJson(JsonVariantConst src, IPAddress& dst) { dst.fromString(src.as<const char*>()); }
OptAddress getConfig(const JsonDocument& doc) {
    if (!doc["static"]) return {};

    IPAddress ip = doc["address"]["ip"];
    IPAddress dns = doc["address"]["dns"];
    IPAddress gateway = doc["address"]["gateway"];
    IPAddress subnet = doc["address"]["subnet"];
    AddressConfig conf{ip, dns, gateway, subnet};
    return conf;
}
void network_handler_deamon(void*) {
    ExFile file, dir;
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
        string m_sinterface = m_json["adapterType"];
        std::transform(m_sinterface.begin(), m_sinterface.end(), m_sinterface.begin(), ::tolower);
        INFO("Its an %s adapter, named: %s", m_sinterface.c_str(), fname);
        string _fname = string(fname);
        if (m_sinterface == "wifi") {
            string ssid = m_json["connection"]["ssid"];
            string pass = m_json["connection"]["password"];
            auto wptr = std::make_shared<WiFiAdapter>(_fname, ssid, pass, m_json["keep-alive"], getConfig(m_json));
            networkManager.add_adapter(wptr);
            processManager.start_process(_fname, wifi_deamon, static_cast<void*>(&_fname));
        }
        if (m_sinterface == "ethernet") {
            networkManager.add_adapter(std::make_shared<EthernetAdapter>(_fname, m_json["keep_alive"]));
            processManager.start_process(_fname, ethernet_deamon, static_cast<void*>(&_fname));
        }
        taskYIELD();
    }
    auto adapter_vector = networkManager.get_adapters();
    auto currentAdaper = networkManager.get_adapter(adapter_vector.at(0));
    while (true) {
        for (auto adapter_name : adapter_vector) {
            auto adapter = networkManager.get_adapter(adapter_name);

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