#pragma once
#include <STM32FreeRTOS.h>
#include <sys/log.h>

#include "../../kernel/network_manager/network_manager.h"
void network_handler_deamon(void*) {
    auto adapter_vector = networkManager.get_adapters();
    INetworkAdapter* currentAdaper =
        networkManager.get_adapter(adapter_vector.at(0));
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
            if (adapter->get_priority() >= currentAdaper->get_priority())
                continue;
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