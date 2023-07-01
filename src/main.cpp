#include <Arduino.h>
// #include <Streaming.h>
// #include <System/DeviceManager/DeviceManager.h>
// #include <System/FileSystem/FileSystem.h>
// #include <System/drivers.h>
//
// DeviceManager* dv = DeviceManager::getInstance();
// FileSystem* fs = FileSystem::getInstance();
//
// DigitalOutput relay;
// rtc* fasz = NULL;
// Screen* fos = NULL;
//
// void setup() {
//     Serial.begin(9600);
//     dev_t rtcid = dv->addDev(DeviceManager::devTypes::rtcdev);
//     dev_t screenid = dv->addDev(DeviceManager::devTypes::screen);
//     fs->mkdir("/dev");
//     fs->mknod("/dev/rtc", rtcid);
//     fs->mknod("/dev/screen", screenid);
//     fasz = static_cast<rtc*>(fs->open("/dev/rtc"));
//     fos = static_cast<Screen*>(fs->open("/dev/screen"));
// }
// void loop() {
//     relay.write(0b11111111);
//     fos->ioctl(Screen::CTRLCMD::CLEAR, 0);
//     fos->write(fasz->read());
//     delay(500);
//     relay.write(0b11000111);
//     delay(500);
// }
#include <SPI.h>
#include <WiFiNINA.h>

#define SPIWIFI SPI
#define SPIWIFI_SS PA4    // Chip select pin
#define SPIWIFI_ACK PB0   // a.k.a BUSY or READY pin
#define ESP32_RESETN PC1  // Reset pin
#define ESP32_GPIO0 -1    // Not connected

void printMacAddress(byte mac[]);
void listNetworks();
void printEncryptionType(int thisType);
void setup() {
    // Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ;  // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("WiFi Scanning test");

    // Set up the pins!
    WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);

    // check for the WiFi module:
    while (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        delay(1000);
    }
    String fv = WiFi.firmwareVersion();
    Serial.println(fv);
    if (fv < "1.0.0") {
        Serial.println("Please upgrade the firmware");
        while (1) delay(10);
    }
    Serial.println("Firmware OK");

    // print your MAC address:
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC: ");
    printMacAddress(mac);
}

void loop() {
    // scan for existing networks:
    Serial.println("Scanning available networks...");
    listNetworks();
    delay(10000);
}

void listNetworks() {
    // scan for nearby networks:
    Serial.println("** Scan Networks **");
    int numSsid = WiFi.scanNetworks();
    if (numSsid == -1) {
        Serial.println("Couldn't get a wifi connection");
        while (true)
            ;
    }

    // print the list of networks seen:
    Serial.print("number of available networks:");
    Serial.println(numSsid);

    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        Serial.print(thisNet);
        Serial.print(") ");
        Serial.print(WiFi.SSID(thisNet));
        Serial.print("\tSignal: ");
        Serial.print(WiFi.RSSI(thisNet));
        Serial.print(" dBm");
        Serial.print("\tEncryption: ");
        printEncryptionType(WiFi.encryptionType(thisNet));
    }
}

void printEncryptionType(int thisType) {
    // read the encryption type and print out the name:
    switch (thisType) {
        case ENC_TYPE_WEP:
            Serial.println("WEP");
            break;
        case ENC_TYPE_TKIP:
            Serial.println("WPA");
            break;
        case ENC_TYPE_CCMP:
            Serial.println("WPA2");
            break;
        case ENC_TYPE_NONE:
            Serial.println("None");
            break;
        case ENC_TYPE_AUTO:
            Serial.println("Auto");
            break;
        case ENC_TYPE_UNKNOWN:
        default:
            Serial.println("Unknown");
            break;
    }
}

void printMacAddress(byte mac[]) {
    for (int i = 5; i >= 0; i--) {
        if (mac[i] < 16) {
            Serial.print("0");
        }
        Serial.print(mac[i], HEX);
        if (i > 0) {
            Serial.print(":");
        }
    }
    Serial.println();
}
