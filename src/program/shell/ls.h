#pragma once
#include <Arduino.h>
#include <Streaming.h>

#include "global_var.h"
#include "kernel/file_system/file_system.h"
#include "sys/log.h"
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