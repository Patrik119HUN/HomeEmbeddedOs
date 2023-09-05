#pragma once
#include <Arduino.h>
#include <Streaming.h>

#include "kernel/file_system/file_system.h"

int lsCallback(int argc, char** argv) {
    for (auto i : fileSystem.lastFilePointer->files) {
        Serial << i->name << endl;
    }
    Serial << ".." << endl;
    return 0;
}