#ifndef buzzer_h
#define buzzer_h
#include <Arduino.h>

extern void beep(uint32_t t_length) {
    pinMode(PB13, OUTPUT);
    digitalWrite(PB13, HIGH);
    delay(t_length);
    digitalWrite(PB13, LOW);
}
#endif // buzzer