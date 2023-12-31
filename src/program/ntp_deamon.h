#pragma once
#include "kernel/kernel.h"
#include <Arduino.h>
#include <NTPClient.h>
#include <STM32FreeRTOS.h>
#include <Streaming.h>
#include <WiFiUdp.h>
void ntp_deamon(void*) {
    UDP* udp = networkManager.getAdapter("esp32")->getUDP();
    NTPClient timeClient(*udp);
    timeClient.begin();
    // IFile* rtc = fileSystem.open("/dev/rtc");
    // if (rtc == nullptr) return 1;
    while (true) {
        if (timeClient.update()) {
            Serial << timeClient.getFormattedTime() << endl;
        }
        // rtc->ioctl(0, timeClient.getEpochTime());
        taskYIELD();
    }
}