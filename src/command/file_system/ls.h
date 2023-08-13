#ifndef ls_h
#define ls_h

#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "kernel/file_system/file_system.h"

void static lsCallback(cmd* c) {
    Command cmd(c);
    for (auto i : fileSystem.lastFilePointer->files) {
        Serial << i->name << endl;
    }
    Serial << ".." << endl;
}
#endif  // ls