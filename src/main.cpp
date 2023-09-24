#include <Arduino.h>
#include <SdFat.h>

#include "kernel/kernel.h"
#include "program/network/ethd.h"
#include "program/network/network_handler_deamon.h"
#include "program/network/ntpd.h"
#include "program/network/wifid.h"
#include "program/shell.h"
#include "secret.h"
#include "sysvar.h"
void setup() {
    Serial.begin(115200);
    if (!ECCX08.begin()) {
        Serial.println("Failed to communicate with ECC508/ECC608!");
        while (1)
            ;
    }
    deviceManager.add_device("rtc", &rtcdev);
    deviceManager.add_device("random", &randomdev);
    deviceManager.add_device("full", &fulldev);
    deviceManager.add_device("zero", &zerodev);
    deviceManager.add_device("null", &nulldev);
    deviceManager.add_device("tty", &term);
    //  tone(PB13, 2000, 100);
    EthernetAdapter ethernetAdapter("w5500");
    WiFiAdapter wifiAdapter("esp32", ssid, psw);
    networkManager.add_adapter(&ethernetAdapter);
    networkManager.add_adapter(&wifiAdapter);
    networkManager.set_stack("w5500");

    processManager.start_process("Shell", shell);
    processManager.start_process("wifid", wifi_deamon);
    processManager.start_process("ethd", ethernet_deamon);
    processManager.start_process("network", network_handler_deamon);
    processManager.start_process("ntpd", ntp_deamon);

    processManager.loop();
}
void loop() {}