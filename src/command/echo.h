#ifndef echo_h
#define echo_h

#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "System/DeviceManager/DeviceManager.h"
#include "System/FileSystem/FileSystem.h"
void static echoCallback(cmd* c) {
    Command cmd(c);
    Argument arg = cmd.getArg(0);
    String message = arg.getValue();

    FileSystem* fs = FileSystem::getInstance();

    int argNum = cmd.countArgs();
    if (argNum == 1) {
        Serial << message << endl;
    }
    if (argNum == 3 && cmd.getArg(1).getValue().compareTo(">") == 0) {
        const char* path = cmd.getArg(2).getValue().c_str();
        Device* scr = fs->open(path);
        if (scr == nullptr) {
            Serial << "no device found!" << endl;
            return;
        }

        scr->ioctl(0, 0);
        Serial << message.length() << endl;
        for (int i = 0; i < message.length(); i++) {

            scr->write(message[i]);
        }
    }
}
#endif  // echo