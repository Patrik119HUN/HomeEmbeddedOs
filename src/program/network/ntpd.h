#pragma once
#include <Arduino.h>
#include <DateTime.h>
#include <STM32FreeRTOS.h>

#include <string>

#include "../../kernel/kernel.h"
#define SEVENZYYEARS 2208988800UL
#define NTP_PACKET_SIZE 48
#define NTP_TIMEOUT_MS 1000
#define PORT 1337
#define NTP_SERVER_PORT 123

void ntp_deamon(void*) {
    IFile* rtc = deviceManager.open("rtc");
    const char* poolServerName = "pool.ntp.org";
    uint32_t currentEpoch = 0;
    uint32_t lastUpdate = 0;
    uint8_t buffer[48];
    UDP* udp;
    unsigned long startAttemptTime;
    DateTime time;
start:
    udp = networkManager.get_udp();
    udp->begin(PORT);
    while (true) {
        while (udp->parsePacket() != 0) udp->flush();
        memset(buffer, 0, NTP_PACKET_SIZE);
        buffer[0] = 0b11100011;  // LI, Version, Mode
        buffer[1] = 0;           // Stratum, or type of clock
        buffer[2] = 6;           // Polling Interval
        buffer[3] = 0xEC;        // Peer Clock Precision
        buffer[12] = 49;
        buffer[13] = 0x4E;
        buffer[14] = 49;
        buffer[15] = 52;
        udp->beginPacket(poolServerName, NTP_SERVER_PORT);
        udp->write(buffer, NTP_PACKET_SIZE);
        udp->endPacket();
        uint8_t timeout = 0;
        int cb = 0;
        do {
            delay(10);
            cb = udp->parsePacket();
            if (timeout > 100) {
                Serial.print("Failed to update");  // timeout after 1000 ms
                goto start;
            }
            timeout++;
        } while (cb == 0);
        udp->read(buffer, NTP_PACKET_SIZE);
        currentEpoch = (makeWord(buffer[40], buffer[41]) << 16 |
                        makeWord(buffer[42], buffer[43])) -
                       SEVENZYYEARS;
        Serial.printf("%d\n",currentEpoch);
        rtc->ioctl(0, currentEpoch);
        vTaskDelay(20000 / portTICK_RATE_MS);
    }
}