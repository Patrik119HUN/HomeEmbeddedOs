#pragma once
#include "kernel/file_system/file_system.h"
#include <Arduino.h>
#include <NTPClient.h>
#include <Streaming.h>
#include <WiFiUdp.h>

int ntp_deamon(int argc, char** argv) {
    UDP* udp = networkManager.getUDP();
    NTPClient timeClient(*udp);
    Serial.print("alma");
    timeClient.begin();
    IFile* rtc = fileSystem.open("/dev/rtc");
    if (rtc == nullptr) return 1;
    while (true) {
        if (!timeClient.update()) {
            Serial << "Failed to update" << endl;
        }
        rtc->ioctl(0, timeClient.getEpochTime());
    }
    return 0;
}