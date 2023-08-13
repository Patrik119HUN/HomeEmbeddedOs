#ifndef cd_h
#define cd_h
#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "kernel/file_system/file_system.h"

void static cdCallback(cmd* c) {
    Command cmd(c);
    Argument arg = cmd.getArg(0);
    String path = arg.getValue();
    if (strcmp(path.c_str(), "..") == 0) {
        fileSystem.lastFilePointer = fileSystem.lastFilePointer->prev_node;
    }
    Node* res = fileSystem.search(path.c_str(), node::Type::FOLDER, fileSystem.lastFilePointer);
    if (res != nullptr) fileSystem.lastFilePointer = res;
    return;
}
#endif  // cd