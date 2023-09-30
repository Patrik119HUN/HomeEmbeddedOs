#include <Arduino.h>
#include <ArduinoJson.h>
#include <SdFat.h>

#include "kernel/kernel.h"
#include "program/network/ethd.h"
#include "program/network/network_handler_deamon.h"
#include "program/network/ntpd.h"
#include "program/network/wifid.h"
#include "program/shell.h"

HardwareSerial Serial1(PA10, PA9);
void setup() {
    Serial.begin(BAUD_RATE);
    Serial1.begin(BAUD_RATE);
    WiFi.init(Serial1);
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println();
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true)
            ;
    }
    if (!ECCX08.begin()) {
        Serial.println("Failed to communicate with ECC508/ECC608!");
        while (1)
            ;
    }
    if (!sd.begin(SdSpiConfig(SD_PIN, SHARED_SPI, SD_SCK_MHZ(16)))) {
        ERROR("Couldn't open SD Card: %d", sd.sdErrorCode());
    }
    deviceManager.add_device("rtc", &rtcdev);
    deviceManager.add_device("random", &randomdev);
    deviceManager.add_device("full", &fulldev);
    deviceManager.add_device("zero", &zerodev);
    deviceManager.add_device("null", &nulldev);
    deviceManager.add_device("tty", &term);
    // tone(PB13, 2000, 100);
    // EthernetAdapter ethernetAdapter("w5500");
    StreamFile sfile(sd.open("wifi1.txt"));
    WiFiAdapter wifiAdapter("esp32", &sfile);
    // networkManager.add_adapter(&ethernetAdapter);
    networkManager.add_adapter(&wifiAdapter);
    // networkManager.set_stack("w5500");
    networkManager.set_stack("esp32");
    processManager.start_process("Shell", shell);
    processManager.start_process("wifid", wifi_deamon);
    // processManager.start_process("ethd", ethernet_deamon);
    // processManager.start_process("network", network_handler_deamon);
    // processManager.start_process("ntpd", ntp_deamon);

    processManager.loop();
}
void loop() {}