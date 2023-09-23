#pragma once
#include <STM32FreeRTOS.h>
#include <sys/log.h>

#include "../kernel/network_manager/network_manager.h"
void network_handler_deamon(void*) {
    auto adapters = networkManager.get_adapters();
    INetworkAdapter* currentAdaper = networkManager.get_adapter(adapters.at(0));
    while (true) {
        for (auto adapter_name : adapters) {
            INetworkAdapter* adapter = networkManager.get_adapter(adapter_name);
            if (adapter == currentAdaper) continue;
            if (adapter->getStatus() == connectionState::CONNECTED) {
                if (currentAdaper->getStatus() == connectionState::CONNECTED) {
                    if (adapter->get_priority() <
                        currentAdaper->get_priority()) {
                        INFO("Changig network to: %s",
                             adapter->getName().c_str());
                        currentAdaper = adapter;
                        networkManager.set_stack(adapter_name);
                    }
                } else {
                    INFO("Changing network to: %s", adapter->getName().c_str());
                    currentAdaper = adapter;
                    networkManager.set_stack(adapter_name);
                }
            }
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