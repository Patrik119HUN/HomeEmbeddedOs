#pragma once
#include "../kernel/kernel.h"
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <string>
#define SEVENZYYEARS 2208988800UL
#define NTP_PACKET_SIZE 48
#define PORT 1337
using std::string;

struct ntp_data {
    bool udpSetup = false;
    string poolServerName = "pool.ntp.org";
    long _timeOffset = 0;
    uint32_t updateInterval = 60000;
    uint32_t currentEpoch = 0;
    uint32_t lastUpdate = 0;
    uint8_t buffer[48];
};
ntp_data data;
void ntp_deamon(void*) {
    UDP* _udp = networkManager.getAdapter("esp32")->getUDP();
    _udp->begin(PORT);
    data.udpSetup = true;
    while (true) {
        while (_udp->parsePacket() != 0)
            _udp->flush();
        memset(data.buffer, 0, NTP_PACKET_SIZE);
        data.buffer[0] = 0b11100011; // LI, Version, Mode
        data.buffer[1] = 0;          // Stratum, or type of clock
        data.buffer[2] = 6;          // Polling Interval
        data.buffer[3] = 0xEC;       // Peer Clock Precision
        data.buffer[12] = 49;
        data.buffer[13] = 0x4E;
        data.buffer[14] = 49;
        data.buffer[15] = 52;
        _udp->beginPacket(data.poolServerName.c_str(), 123);
        _udp->write(data.buffer, NTP_PACKET_SIZE);
        _udp->endPacket();
        uint8_t timeout = 0;
        int cb = 0;
        do {
            delay(10);
            cb = _udp->parsePacket();
            if (timeout > 100) {
                Serial.printf("Failed to update\n");
            }
            timeout++;
        } while (cb == 0);

        data.lastUpdate = millis() - (10 * (timeout + 1));

        _udp->read(data.buffer, NTP_PACKET_SIZE);

        uint32_t highWord = word(data.buffer[40], data.buffer[41]);
        uint32_t lowWord = word(data.buffer[42], data.buffer[43]);
        uint32_t secsSince1900 = highWord << 16 | lowWord;

        data.currentEpoch = secsSince1900 - SEVENZYYEARS +
                            ((millis() - data.lastUpdate) / 1000); // Time since last update

        Serial.printf("%d\n", data.currentEpoch);
        taskYIELD();
    }
    _udp->stop();
    data.udpSetup = false;
}