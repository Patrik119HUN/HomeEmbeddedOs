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
            Serial.printf("%s : %s\n", path_s().c_str(), out.c_str());
            auto texts = tokenize_str(out, ' ');
            string cmd = texts.at(0);
            if (cmd == "cd") {
                processManager.startProcess("cd", cdCallback,static_cast<void*>(&texts.at(1)));
            } else if (cmd == "ls") {
                processManager.startProcess("ls", lsCallback);
            } else if (cmd == "pwd") {
                processManager.startProcess("pwd",pwdCallback);
            } else if (cmd == "mkdir") {
                processManager.startProcess("mkdir", mkdirCallback,static_cast<void*>(&texts.at(1)));
            } else if (cmd == "rmdir") {
                processManager.startProcess("rmdir", rmdirCallback,static_cast<void*>(&texts.at(1)));
            } else {
                Serial.println("cmd not found");
            }
        }
        taskYIELD();
    }
}