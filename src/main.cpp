#include "sysvar.h"

#include "kernel/kernel.h"

#include "program/dummy.h"
#include "program/ethd.h"
#include "program/network_handler_deamon.h"
#include "program/ntp_deamon.h"
#include "program/print_random.h"
#include "program/shell.h"
#include "program/wifid.h"
#include <Arduino.h>

#include "secret.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <SdFat.h>
void setup() {
    Serial.begin(9600);
    if (!ECCX08.begin()) {
        Serial.println("Failed to communicate with ECC508/ECC608!");
        while (1)
            ;
    }
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Booting");
    display.display();

    deviceManager.addDevice("rtc", &rtcdev);
    deviceManager.addDevice("random", &randomdev);
    deviceManager.addDevice("full", &fulldev);
    deviceManager.addDevice("zero", &zerodev);
    deviceManager.addDevice("null", &nulldev);
    deviceManager.addDevice("tty", &term);
    fileSystem.mkdir("/dev");
    fileSystem.mkdir("/user");
    // tone(PB13, 2000, 100);

    EthernetAdapter ethernetAdapter("w5500");
    WiFiAdapter wifiAdapter("esp32", ssid, psw);
    networkManager.addAdapter(&ethernetAdapter);
    networkManager.addAdapter(&wifiAdapter);
    networkManager.setStack("w5500");

    xTaskCreate(shell, "Shell", configMINIMAL_STACK_SIZE + 800, NULL, 1, NULL);
    xTaskCreate(wifi_deamon, "wifid", 5000, NULL, 1, NULL);
    xTaskCreate(ntp_deamon, "ntpd", 1000, NULL, 1, NULL);

    processManager.startProcess("ethernet_deamon", ProcessPriority::ABOVE_BASE, ethernet_deamon, 0);
    // processManager.startProcess("networkManager", ProcessPriority::ABOVE_BASE,
    // network_handler_deamon,0);
    processManager.loop();
}
void loop() {}