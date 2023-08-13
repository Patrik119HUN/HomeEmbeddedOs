#ifndef cli_deamon_h
#define cli_deamon_h

#include "../command/commands.h"
#include "../sysheaders.h"
#include <Arduino.h>
#include <SimpleCLI.h>
#include <Streaming.h>

SimpleCLI cli;
Command ls;
Command cd;
Command mkdir;
Command echo;

void errorCallback(cmd_error* e);
int cli_deamon(int argc, char** argv) {
    FileSystem* fs = FileSystem::getInstance();
    cli.setOnError(errorCallback);
    ls = cli.addCommand("ls", lsCallback);
    cd = cli.addSingleArgumentCommand("cd", cdCallback);
    mkdir = cli.addSingleArgumentCommand("mkdir", mkdirCallback);
    echo = cli.addCommand("echo", echoCallback);
    echo.addPositionalArgument("str", "pon");
    echo.addPositionalArgument("p", "/dev/tty");

    bool arrived = true;
    while (true) {
        if (arrived) {
            Serial << fs->currentPath(fs->lastFilePointer) << "> ";
            arrived = false;
        }
        if (Serial.available()) {
            String input = Serial.readStringUntil('\n');
            Serial << "#" << input << endl;
            cli.parse(input);
            arrived = true;
        }
    }
    return 0;
}
void errorCallback(cmd_error* e) {
    CommandError cmdError(e);

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}
#endif // clie_deamon