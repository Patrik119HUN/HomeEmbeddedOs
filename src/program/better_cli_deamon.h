#pragma once
#include <Arduino.h>
#include <cmdArduino.h>
#include "ls.h"
#include "cd.h"
#include "restart.h"
int cli_deamon(int argc, char** argv) {

    cmd.begin(9600);
    cmd.add("cd", cdCallback);
    cmd.add("ls", lsCallback);
    cmd.add("restart", restartCallback);

    while (true) {
        cmd.poll();
    }
    return 0;
}