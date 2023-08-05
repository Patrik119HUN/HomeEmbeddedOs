#ifndef cd_h
#define cd_h
#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "kernel/file_system/file_system.h"

void static cdCallback(cmd* c) {
    Command cmd(c);
    FileSystem* fs = FileSystem::getInstance();
    Argument arg = cmd.getArg(0);
    String path = arg.getValue();
    if (strcmp(path.c_str(), "..") == 0) {
        fs->lastFilePointer = fs->lastFilePointer->prev_node;
    }
    Node* res = fs->search(path.c_str(), node::Type::FOLDER, fs->lastFilePointer);
    if (res != nullptr) fs->lastFilePointer = res;
    return;
}
#endif  // cd