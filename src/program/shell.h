#pragma once
#include "../kernel/kernel.h"
#include "../kernel/sysmacros.h"
#include "shell/cd.h"
#include "shell/global_var.h"
#include "shell/ls.h"
#include "shell/restart.h"
#include <Arduino.h>
#include <sys/log.h>

using std::string;

string out;
char buffer[256];
void shell(void*) {
    if (!sd.begin(SdSpiConfig(SD_PIN, SHARED_SPI, SD_SCK_MHZ(16)))) {
        syslog(&Serial, Debug_level::ERROR, "Couldn't open Sd card: %d", sd.sdErrorCode());
        vTaskDelete(NULL);
    }

    while (true) {
        int r = Serial.readBytesUntil('\n', buffer, 256);

        if (r != 0) {
            Serial.print(path_s().c_str());
            Serial.print(" > ");
            out.assign(buffer, r);
            Serial.println(out.c_str());

            auto texts = tokenize_str(out, ' ');
            string cmd = texts.at(0);
            if (cmd == "cd") {
                xTaskCreate(
                    cdCallback, "cd", configMINIMAL_STACK_SIZE + 800,
                    static_cast<void*>(&texts.at(1)), 1, NULL
                );
            } else if (cmd == "ls") {
                xTaskCreate(lsCallback, "ls", configMINIMAL_STACK_SIZE + 800, NULL, 1, NULL);
            } else {
                Serial.println("cmd not found");
            }
        }
        taskYIELD();
    }
}