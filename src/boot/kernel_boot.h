#ifndef kernel_boot_h
#define kernel_boot_h
#include "includes.h"

#include <Arduino.h>

#include "program/cli_deamon.h"
#include "program/ntp_deamon.h"
extern void kernel_boot(void) {
    Serial.begin(BAUD_RATE);
    VolumeManager* vm = VolumeManager::getInstance();

    vm->mount("C", FSType::FAT32, &SD);

    fileSystemInit();
    beep(100);
    Ethernet.init(W5500_PIN);
    if (!SD.begin(SD_PIN)) {
        DEBUG_ERROR("SD initialization failed");
        while (1)
            ;
    }
    DEBUG_INFO("SD initialization succed");

    WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
    WiFi.begin("Wi-Fi", "Asdfghjkl12");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    EthernetConnectionHandler ethernet;
    
    NetworkManager::getInstance()->addHandler(&wifi);
    NetworkManager::getInstance()->addHandler(&ethernet);
    ProcessManager::getInstance()->startProcess("ntp_deamon", false, &ntp_deamon, 0);
    ProcessManager::getInstance()->startProcess("cli_deamon", true, &cli_deamon, 0);
    // ProcessManager::getInstance()->startProcess("cli", true, &ntp_deamon, 1);
}
#endif // kernel_boot