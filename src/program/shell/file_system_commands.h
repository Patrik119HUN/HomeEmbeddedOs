#pragma once
#include "global_var.h"
#include "kernel/file_system/file_system.h"
#include "sys/log.h"
#include <Arduino.h>
#include <SdFat.h>

void cdCallback(void* arg) {
    std::string& folder = *(std::string*)arg;
    ExFile file;
    ExFile dir;
    if (folder == "..") paths.pop_back();
    if (!dir.open(path_s().c_str())) {
        syslog(&Serial, Debug_level::ERROR, "Failed to open sd");
    }
    while (file.openNext(&dir, O_RDONLY)) {
        char fname[16];
        file.getName(fname, 16);
        if (folder == fname) {
            if (file.isDir()) {
                paths.push_back(folder);
            }
        }
        file.close();
    }
    vTaskDelete(NULL);
}

void lsCallback(void*) {
    ExFile file;
    ExFile dir;
    if (!dir.open(path_s().c_str())) {
        syslog(&Serial, Debug_level::ERROR, "Failed to open sd");
    }
    while (file.openNext(&dir, O_RDONLY)) {
        file.printFileSize(&Serial);
        Serial.print(' ');
        file.printModifyDateTime(&Serial);
        Serial.print(' ');
        file.printName(&Serial);
        if (file.isDir()) {
            Serial.print('/');
        }
        Serial.println();
        file.close();
    }
    vTaskDelete(NULL);
}

void mkdirCallback(void* arg) {
    std::string& folder = *(std::string*)arg;

    sd.mkdir(folder.c_str());
    vTaskDelete(NULL);
}
void rmdirCallback(void* arg) {
    std::string& folder = *(std::string*)arg;

    sd.rmdir(folder.c_str());
    vTaskDelete(NULL);
}
void pwdCallback(void* arg) {
    Serial.println(path_s().c_str());
    vTaskDelete(NULL);
}