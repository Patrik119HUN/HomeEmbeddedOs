#include "sysvar.h"

#include "kernel/kernel.h"

#include "program/ethd.h"
#include "program/network_handler_deamon.h"
#include "program/ntp_deamon.h"
#include "program/print_random.h"
#include "program/shell.h"
#include "program/wifid.h"
#include <Arduino.h>

#include "secret.h"
#include <SdFat.h>
void setup() {
    Serial.begin(115200);
    if (!ECCX08.begin()) {
        Serial.println("Failed to communicate with ECC508/ECC608!");
        while (1)
            ;
    }
    deviceManager.addDevice("rtc", &rtcdev);
    deviceManager.addDevice("random", &randomdev);
    deviceManager.addDevice("full", &fulldev);
    deviceManager.addDevice("zero", &zerodev);
    deviceManager.addDevice("null", &nulldev);
    deviceManager.addDevice("tty", &term);
    //  tone(PB13, 2000, 100);
    EthernetAdapter ethernetAdapter("w5500");
    WiFiAdapter wifiAdapter("esp32", ssid, psw);
    networkManager.addAdapter(&ethernetAdapter);
    networkManager.addAdapter(&wifiAdapter);
    networkManager.setStack("w5500");

    processManager.startProcess("Shell", shell);
    processManager.startProcess("wifid", wifi_deamon);
    processManager.startProcess("ethd", ethernet_deamon);
    processManager.startProcess("ntpd", ntp_deamon);

    processManager.loop();
}
void loop() {}