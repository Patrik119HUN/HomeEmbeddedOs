#pragma once
#include "../kernel/kernel.h"
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <sys/log.h>
#include <sysvar.h>

#define TASK_SEC 1000 / portTICK_PERIOD_MS
#define WIFI_TIMEOUT_MS 20000
#define LOG_WARNING(string) syslog(&Serial, Debug_level::WARNING, string)
#define LOG_ERROR(string) syslog(&Serial, Debug_level::ERROR, string)

unsigned long startAttemptTime;

void wifi_deamon(void*) {
    WiFiAdapter* wifi = static_cast<WiFiAdapter*>(networkManager.getAdapter("esp32"));
    if (wifi->begin() == 1) {
        LOG_ERROR("Wi-Fi was not found.");
        wifi->setStatus(connectionState::ERROR);
    }
    while (true) {
        if (WiFi.status() == WL_CONNECTED) {
            LOG_WARNING("Wi-Fi connected");
            Serial.println(WiFi.localIP());
            wifi->setStatus(connectionState::CONNECTED);
            vTaskDelay(20 * TASK_SEC);
            continue;
        }
        auto [ssid, pass] = wifi->getConnectionData();
        WiFi.begin(ssid.c_str(), pass.c_str());
        startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
            taskYIELD();
        }

        if (WiFi.status() != WL_CONNECTED) {
            wifi->listNetworks();
            LOG_WARNING("Waiting Wi-Fi configuration from DHCP server, check cable connection");
            wifi->setStatus(connectionState::DISCONNECTED);
            vTaskDelay(5 * TASK_SEC);
            continue;
        }
        wifi->setStatus(connectionState::CONNECTED);
        taskYIELD();
    }
}