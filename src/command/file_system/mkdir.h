#ifndef mkdir_h
#define mkdir_h

#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "kernel/file_system/file_system.h"

void static mkdirCallback(cmd* c) {
    Command cmd(c);
    FileSystem* fs = FileSystem::getInstance();
    Argument arg = cmd.getArg(0);
    const char* name = arg.getValue().c_str();

    fs->mkdir(name);
}
#endif  // mkdir