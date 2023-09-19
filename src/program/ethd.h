#pragma once
#include "../kernel/kernel.h"
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <sys/log.h>

#define LOG_WARNING(string) syslog(&Serial, Debug_level::WARNING, string)
#define LOG_ERROR(string) syslog(&Serial, Debug_level::ERROR, string)
const TickType_t xDelay = 2000 / portTICK_PERIOD_MS;

void ethernet_deamon(void*) {
    bool printed = false;
    EthernetAdapter* eth = (EthernetAdapter*)networkManager.getAdapter("w5500");
    switch (eth->begin()) {
    case 1:
        LOG_ERROR("Ethernet shield was not found.");
        eth->setStatus(connectionState::ERROR);
        break;
    case 2:
        LOG_WARNING("Failed to configure Ethernet, check cable connection");
        eth->setStatus(connectionState::CONNECTING);
        break;
    case 3:
        LOG_WARNING("Waiting Ethernet configuration from DHCP server, check cable connection");
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
            LOG_WARNING("Ethernet link OFF, connection lost.");
        }
        eth->setStatus(connectionState::DISCONNECTED);
        vTaskDelay(xDelay);
        taskYIELD();
    }
}