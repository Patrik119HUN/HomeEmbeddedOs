#ifndef cd_h
#define cd_h
#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "System/FileSystem/FileSystem.h"

void cdCallback(cmd* c) {
    Command cmd(c);
    FileSystem* fs = FileSystem::getInstance();
    Argument arg = cmd.getArg(0);
    String path = arg.getValue();
    if (strcmp(path.c_str(), "..") == 0) {
        fs->lastFilePointer = fs->lastFilePointer->prevNode;
    }
    node* res = fs->search(path.c_str(), nodeType::FOLDER, fs->lastFilePointer);
    if (res != nullptr) fs->lastFilePointer = res;
    return;
}
#endif  // cd