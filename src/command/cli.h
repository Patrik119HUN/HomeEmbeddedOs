#ifndef cli_h
#define cli_h

#include <Arduino.h>
#include <SimpleCLI.h>

#include "FileSystem/FileSystem.h"
class cli {
   private:
    SimpleCLI cli;
    Command sum;
    Command ls;
    Command cd;
    Command mkdir;

    bool arrived = true;

    void errorCallback(cmd_error* e) {
        CommandError cmdError(e);  // Create wrapper object

        Serial.print("ERROR: ");
        Serial.println(cmdError.toString());

        if (cmdError.hasCommand()) {
            Serial.print("Did you mean \"");
            Serial.print(cmdError.getCommand().toString());
            Serial.println("\"?");
        }
    }
    /* data */
   public:
    cli() {
        cli.setOnError(errorCallback);  // Set error Callback
        sum = cli.addBoundlessCommand("sum", sumCallback);
        ls = cli.addCommand("ls", lsCallback);
        cd = cli.addSingleArgumentCommand("cd", cdCallback);
        mkdir = cli.addSingleArgumentCommand("mkdir", mkdirCallback);
    }
    ~cli();

    void loop() {}
    if (arrived) {
        Serial << endl;
        Serial << fs->currentPath(fs->lastFilePointer) << "> ";
        arrived = false;
    }
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
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
};

#endif  // cli