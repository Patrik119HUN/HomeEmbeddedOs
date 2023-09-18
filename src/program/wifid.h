#pragma once
#include "../kernel/kernel.h"
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <sys/log.h>

#define WIFI_TIMEOUT_MS 20000
#define LOG_WARNING(string) syslog(&Serial, Debug_level::WARNING, string)
#define LOG_ERROR(string) syslog(&Serial, Debug_level::ERROR, string)

unsigned long startAttemptTime;

void wifi_deamon(void*) {
    bool printedConnected = false, printedDisconnected = false;
    WiFiAdapter* wifi = (WiFiAdapter*)networkManager.getAdapter("esp32");
    switch (wifi->begin()) {
    case 1:
        LOG_ERROR("Wi-Fi was not found.");
        wifi->setStatus(connectionState::ERROR);
        break;
    default:
        break;
    }
    while (true) {
        if (WiFi.status() == WL_CONNECTED) {
            if (!printedConnected) {
                printedConnected = true;
                printedDisconnected = false;
                LOG_WARNING("Wi-Fi connected");
                Serial.println(WiFi.localIP());
            }
            wifi->setStatus(connectionState::CONNECTED);
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
        auto [ssid, pass] = wifi->getConnectionData();
        WiFi.begin(ssid.c_str(), pass.c_str());
        startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
            taskYIELD();
        }

        if (WiFi.status() != WL_CONNECTED) {
            if (!printedDisconnected) {
                printedDisconnected = true;
                printedConnected = false;
                LOG_WARNING("Waiting Wi-Fi configuration from DHCP server, check cable connection");
            }
            wifi->setStatus(connectionState::DISCONNECTED);
            vTaskDelay(20000 / portTICK_PERIOD_MS);
            continue;
        }
        wifi->setStatus(connectionState::CONNECTED);
        taskYIELD();
    }
}