#pragma once
#include "../kernel/kernel.h"
#include "../kernel/sysmacros.h"
#include "shell/file_system_commands.h"
#include "shell/global_var.h"
#include "shell/restart.h"
#include <Arduino.h>
#include <sys/log.h>

using std::string;

string out;
char buffer[256];
size_t read;
void shell(void*) {
    if (!sd.begin(SdSpiConfig(SD_PIN, SHARED_SPI, SD_SCK_MHZ(16)))) {
        syslog(&Serial, Debug_level::ERROR, "Couldn't open Sd card: %d", sd.sdErrorCode());
        vTaskDelete(NULL);
    }

    while (true) {
        read = Serial.readBytesUntil('\n', buffer, 256);

        if (read != 0) {
            out.assign(buffer, read);
            Serial.printf("%s : %s", path_s().c_str(), out.c_str());
            auto texts = tokenize_str(out, ' ');
            string cmd = texts.at(0);
            if (cmd == "cd") {
                xTaskCreate(
                    cdCallback, "cd", configMINIMAL_STACK_SIZE + 800,
                    static_cast<void*>(&texts.at(1)), 1, NULL
                );

            } else if (cmd == "ls") {
                xTaskCreate(lsCallback, "ls", configMINIMAL_STACK_SIZE + 800, NULL, 1, NULL);
            } else if (cmd == "pwd") {
                xTaskCreate(pwdCallback, "pwd", configMINIMAL_STACK_SIZE + 800, NULL, 1, NULL);
            } else if (cmd == "mkdir") {
                xTaskCreate(
                    mkdirCallback, "mkdir", configMINIMAL_STACK_SIZE + 800,
                    static_cast<void*>(&texts.at(1)), 1, NULL
                );
            } else if (cmd == "rmdir") {
                xTaskCreate(
                    rmdirCallback, "rmdir", configMINIMAL_STACK_SIZE + 800,
                    static_cast<void*>(&texts.at(1)), 1, NULL
                );
            } else {
                Serial.println("cmd not found");
            }
        }
        taskYIELD();
    }
}