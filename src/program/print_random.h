#include <Arduino.h>
#include <Streaming.h>
#include <uuid.h>
int randomGen(int argc, char** argv) {
    UUID id;
    while (true) {
        Serial.println(id.getUUID());
        delay(100);
    }

    return 0;
}