#pragma once
#include "../kernel/kernel.h"
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <ethernet/ethernet_adapter.h>
#include <sys/log.h>

#define LOG_WARNING(string) syslog(&Serial, Debug_level::WARNING, string)
#define LOG_ERROR(string) syslog(&Serial, Debug_level::ERROR, string)
const TickType_t xDelay = 2000 / portTICK_PERIOD_MS;
void ethernet_deamon(void*) {
    INetworkAdapter* eth = networkManager.getAdapter("w5500");
    switch (eth->begin()) {
    case 1:
        LOG_ERROR("Ethernet shield was not found.");
        eth->setStatus(NetworkConnectionState::ERROR);
        break;
    case 2:
        LOG_WARNING("Failed to configure Ethernet, check cable connection");
        eth->setStatus(NetworkConnectionState::CONNECTING);
        break;
    case 3:
        LOG_WARNING("Waiting Ethernet configuration from DHCP server, check cable connection");
        eth->setStatus(NetworkConnectionState::CONNECTING);
        break;
    default:
        break;
    }
    while (true) {
        if (Ethernet.linkStatus() == LinkOFF || Ethernet.linkStatus() == Unknown) {
            LOG_WARNING("Ethernet link OFF, connection lost.");
            eth->setStatus(NetworkConnectionState::DISCONNECTED);
        } else {
            eth->setStatus(NetworkConnectionState::CONNECTED);
        }
        vTaskDelay(xDelay);
        taskYIELD();
    }
}