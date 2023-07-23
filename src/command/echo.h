#ifndef echo_h
#define echo_h

#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "System/DeviceManager/DeviceManager.h"
#include "System/FileSystem/FileSystem.h"
void static echoCallback(cmd* c) {
    Command cmd(c);
    Argument arg = cmd.getArgument("str");
    Argument display = cmd.getArgument("p");

    String message = arg.getValue();
    String da = display.getValue();

    FileSystem* fs = FileSystem::getInstance();
    IFile* scr = fs->open(da.c_str());

    if (scr == nullptr) return;

    scr->ioctl(0, 0);
    for (size_t i = 0; i < message.length(); i++) scr->write(message[i]);
    scr->write('\n');
    return;
}
#endif  // echo