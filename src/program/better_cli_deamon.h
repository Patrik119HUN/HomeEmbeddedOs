#pragma once
#include "cd.h"
#include "ls.h"
#include "restart.h"
#include <Arduino.h>
#include <cmdArduino.h>
#include "../kernel/kernel.h"
int cli_deamon(int argc, char** argv) {

    cmd.begin(9600);
    cmd.add("cd", cdCallback);
    cmd.add("ls", lsCallback);
    cmd.add("restart", restartCallback);

    while (true) {
        if (Serial.available()) {
            cmd.handler();
        }
    }
    return 0;
}