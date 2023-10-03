#pragma once
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <sys/log.h>

#include "../../kernel/kernel.h"

#define TASK_SEC 1000 / portTICK_PERIOD_MS
#define WIFI_TIMEOUT_MS 10000
unsigned long startAttemptTime;

void wifi_deamon(void* arg) {
    auto getNetwork = networkManager.get_adapter(*static_cast<std::string*>(arg));
    auto wifi = std::dynamic_pointer_cast<WiFiAdapter>(getNetwork);
    if (wifi->begin() == 1) {
        ERROR("Wi-Fi was not found.");
        wifi->setStatus(connectionState::ERROR);
        vTaskDelete(NULL);
    }
    while (true) {
        if (WiFi.status() == WL_CONNECTED) {
            wifi->setStatus(connectionState::CONNECTED);
            vTaskDelay(20 * TASK_SEC);
            continue;
        }
        auto [ssid, pass] = wifi->getConnectionData();
        INFO("Connecting to %s %s", ssid.c_str(), pass.c_str());
        WiFi.begin(ssid.c_str(), pass.c_str());
        startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
            taskYIELD();
        }

        if (WiFi.status() != WL_CONNECTED) {
            WARNING(
                "Waiting Wi-Fi configuration from DHCP server, check cable "
                "connection");
            wifi->setStatus(connectionState::DISCONNECTED);
            vTaskDelay(5 * TASK_SEC);
            continue;
        }
        wifi->setStatus(connectionState::CONNECTED);
        taskYIELD();
    }
}