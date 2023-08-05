#ifndef bus_out_h
#define bus_out_h
#include <Arduino.h>

const uint8_t PIN_NUMBER = 8;
class BusOut {
  public:
    BusOut(uint32_t* t_pins) {
        m_pins = t_pins;
        uint8_t length = sizeof(t_pins) / sizeof(t_pins[0]);
        for (uint8_t i = 0; i < length; i++) {
            if (m_pins[i] != NC) {
                pinMode(m_pins[i], OUTPUT);
            }
        }
    };
    void busWrite(const int data) {
        for (uint8_t i = 0; i < PIN_NUMBER; i++) {
            digitalWrite(m_pins[i], bitRead(data, i));
        }
    }

  private:
    uint32_t* m_pins;
};

#endif // bus_out