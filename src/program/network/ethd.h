#pragma once
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <sys/log.h>

#include "../../kernel/kernel.h"

const TickType_t xDelay = 2000 / portTICK_PERIOD_MS;

void ethernet_deamon(void* adapter) {
    bool printed = false;
    EthernetAdapter* eth = static_cast<EthernetAdapter*>(adapter);

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
        vTaskDelay(xDelay);
        taskYIELD();
    }
}