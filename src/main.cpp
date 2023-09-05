#include "sysvar.h"

#include "kernel/kernel.h"

#include "program/better_cli_deamon.h"
#include "program/ntp_deamon.h"
#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ethernet_handler/ethernet_handler.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup() {
    Debug.timestampOn();
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.print("Booting");
    display.display();
    Debug.setDebugLevel(DBG_VERBOSE);
    fileSystem.mkdir("/dev");
    fileSystem.mknod("/dev/rtc", deviceManager.addDevice(DeviceTypes::REAL_TIME_CLOCK, &rtcdev));
    fileSystem.mknod("/dev/full", deviceManager.addDevice(DeviceTypes::SYSTEM, &fulldev));
    fileSystem.mknod("/dev/zero", deviceManager.addDevice(DeviceTypes::SYSTEM, &nulldev));
    fileSystem.mknod("/dev/null", deviceManager.addDevice(DeviceTypes::SYSTEM, &zerodev));
    fileSystem.mknod("/dev/tty", deviceManager.addDevice(DeviceTypes::SCREEN, &term));
    fileSystem.mkdir("/user");
    processSettings cli{"cli_deamon", ProcessPriority::BASE};
    processSettings ntp{"ntp_deamon", ProcessPriority::BASE};
    // processSettings network{"network_deamon", ProcessPriority::BASE};
    // processSettings dummy_settings{"dummy", ProcessPriority::BASE};
    tone(PB13, 2000, 100);
    Ethernet.init(W5500_PIN);
    if (!SD.begin(SD_PIN)) {
        DEBUG_ERROR("SD initialization failed");
        while (1)
            ;
    }
    volumeManager.mount("C", FSType::FAT32, &SD);

    EthernetConnectionHandler EthernetAdapter;

    networkManager.addAdapter(&EthernetAdapter);
    networkManager.setClient(&EthernetAdapter.getClient());
    networkManager.setUDP(&EthernetAdapter.getUDP());
    display.clearDisplay();
    display.display();

    // processManager.startProcess(&ntp, &ntp_deamon, 0);
    processManager.startProcess(&cli, &cli_deamon, 0);
}
void loop() { processManager.loop(); }