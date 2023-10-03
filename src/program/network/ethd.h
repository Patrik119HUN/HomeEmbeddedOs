#pragma once
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <sys/log.h>

#include "../../kernel/kernel.h"

void ethernet_deamon(void* arg) {
    bool printed = false;
    auto getNetwork = networkManager.get_adapter(*static_cast<std::string*>(arg));
    auto eth = std::dynamic_pointer_cast<EthernetAdapter>(getNetwork);

    switch (eth->begin()) {
        case 1:
            ERROR("Ethernet shield was not found.");
            eth->setStatus(connectionState::ERROR);
            break;
        case 2:
            WARNING("Failed to configure Ethernet, check cable connection");
            eth->setStatus(connectionState::CONNECTING);
            break;
        case 3:
            WARNING("Waiting Ethernet configuration from DHCP server, check cable connection");
            eth->setStatus(connectionState::CONNECTING);
            break;
        default:
            break;
    }
    while (true) {
        if (Ethernet.linkStatus() == LinkON) {
            printed = false;
            eth->setStatus(connectionState::CONNECTED);
            taskYIELD();
            continue;
        }
        if (!printed) {
            printed = true;
            WARNING("Ethernet link OFF, connection lost.");
        }
        eth->setStatus(connectionState::DISCONNECTED);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        taskYIELD();
    }
}