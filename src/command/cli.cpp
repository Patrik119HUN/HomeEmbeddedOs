#include "cli.h"

#include "FileSystem/cd.h"
#include "FileSystem/ls.h"
#include "FileSystem/mkdir.h"
#include "echo.h"
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
void console::loop() {
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
    if (cli.errored()) {
        CommandError cmdError = cli.getError();
        Serial.print("ERROR: ");
        Serial.println(cmdError.toString());
        if (cmdError.hasCommand()) {
            Serial.print("Did you mean \"");
            Serial.print(cmdError.getCommand().toString());
            Serial.println("\"?");
        }
    }
}
console::console() {
    cli.setOnError(errorCallback);
    ls = cli.addCommand("ls", lsCallback);
    cd = cli.addSingleArgumentCommand("cd", cdCallback);
    mkdir = cli.addSingleArgumentCommand("mkdir", mkdirCallback);
    echo = cli.addCommand("echo", echoCallback);
    echo.addPositionalArgument("str","pon");
    echo.addPositionalArgument("p","/dev/tty");
}
