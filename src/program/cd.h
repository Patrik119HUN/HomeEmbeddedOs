#pragma once
#include <Arduino.h>
#include <Streaming.h>

#include "kernel/file_system/file_system.h"

int cdCallback(int argc,char** argv) {
    if (strcmp(argv[1], "..") == 0) {
        fileSystem.lastFilePointer = fileSystem.lastFilePointer->prev_node;
    }
    Node* res = fileSystem.search(argv[1], node::Type::FOLDER, fileSystem.lastFilePointer);
    if (res != nullptr) fileSystem.lastFilePointer = res;
    return 0;
}