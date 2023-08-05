#ifndef cli_h
#define cli_h

#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "../kernel/file_system/file_system.h"
void static errorCallback(cmd_error* e);
class console {
   private:
    SimpleCLI cli;
    Command ls, cd, mkdir;
    Command echo;

    bool arrived = true;

    FileSystem* fs = FileSystem::getInstance();

   public:
    console();
    void loop();
};

#endif  // cli