#ifndef kernel_boot_h
#define kernel_boot_h
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
    VolumeManager* vm = VolumeManager::getInstance();

    vm->mount("C", FSType::FAT32, &SD);

    fileSystemInit();

    tone(PB13, 2000, 200);
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
    processManager.startProcess("cli_deamon", true, &cli_deamon, 0);
}
#endif // kernel_boot