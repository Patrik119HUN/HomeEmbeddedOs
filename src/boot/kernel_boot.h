#pragma once
#include "includes.h"

#include "program/cli_deamon.h"
#include "program/ntp_deamon.h"
#include <Arduino.h>
#include <WiFi.h>
#define SPIWIFI SPI
#define SPIWIFI_SS PA4   // Chip select pin
#define SPIWIFI_ACK PB0  // a.k.a BUSY or READY pin
#define ESP32_RESETN PC1 // Reset pin
#define ESP32_GPIO0 -1   // Not connected
extern void kernel_boot(void) {
    Serial.begin(BAUD_RATE);
    volumeManager.mount("C", FSType::FAT32, &SD);

    fileSystem.mkdir("/dev");
    fileSystem.mknod("/dev/rtc", deviceManager.addDevice(DeviceTypes::REAL_TIME_CLOCK, &rtcdev));
    fileSystem.mknod("/dev/full", deviceManager.addDevice(DeviceTypes::SYSTEM, &fulldev));
    fileSystem.mknod("/dev/zero", deviceManager.addDevice(DeviceTypes::SYSTEM, &nulldev));
    fileSystem.mknod("/dev/null", deviceManager.addDevice(DeviceTypes::SYSTEM, &zerodev));
    fileSystem.mknod("/dev/tty", deviceManager.addDevice(DeviceTypes::SCREEN, &term));
    fileSystem.mkdir("/user");

    processManager.startProcess("cli_deamon", true, &cli_deamon, 0);
    tone(PB13, 2000, 100);
    Ethernet.init(W5500_PIN);
    if (!SD.begin(SD_PIN)) {
        DEBUG_ERROR("SD initialization failed");
        while (1)
            ;
    }
    WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
    WiFi.begin("Wi-Fi", "Asdfghjkl12");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    processManager.startProcess("ntp_deamon", false, &ntp_deamon, 0);
}