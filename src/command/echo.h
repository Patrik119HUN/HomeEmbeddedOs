#pragma once
#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

#include "kernel/device_manager/device_manager.h"
#include "kernel/file_system/file_system.h"
void static echoCallback(cmd* c) {
    Command cmd(c);
    Argument arg = cmd.getArgument("str");
    Argument display = cmd.getArgument("p");

    String message = arg.getValue();
    String da = display.getValue();

    IFile* scr = fileSystem.open(da.c_str());

    if (scr == nullptr) return;

    scr->ioctl(0, 0);
    for (size_t i = 0; i < message.length(); i++)
        scr->write(message[i]);
    scr->write('\n');
    return;
}