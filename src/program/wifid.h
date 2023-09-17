#pragma once
#include "../kernel/kernel.h"
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <sys/log.h>

#define LOG_WARNING(string) syslog(&Serial, Debug_level::WARNING, string)
#define LOG_ERROR(string) syslog(&Serial, Debug_level::ERROR, string)

void wifi_deamon(void*) {
    WiFiAdapter* wifi = (WiFiAdapter*)networkManager.getAdapter("esp32");
    switch (wifi->begin()) {
    case 1:
        LOG_ERROR("Wi-Fi was not found.");
        wifi->setStatus(connectionState::ERROR);
        break;
    case 2:
        LOG_WARNING("Waiting Wi-Fi configuration from DHCP server, check cable connection");
        wifi->setStatus(connectionState::CONNECTING);
        break;
    default:
        break;
    }
    while (true) {
        if (WiFi.status() != WL_CONNECTED && wifi->reconnect() == 2) {
            LOG_WARNING("Waiting Wi-Fi configuration from DHCP server, check cable connection");
            wifi->setStatus(connectionState::DISCONNECTED);
        }
        wifi->setStatus(connectionState::CONNECTED);
        vTaskDelay(xDelay);
        taskYIELD();
    }
}