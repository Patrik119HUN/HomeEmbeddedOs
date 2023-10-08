#pragma once

#include <Arduino.h>
#include <file_interface.h>

#include "bus_out.h"
class DigitalOutput : public IFile {
   public:
    enum DOCMD { SET_ALL, RESET_ALL };
    DigitalOutput() {
        m_name = "Digital_Output";
        m_relay = new BusOut(m_pins);
        setTimeout(0);
    }
    int ioctl(int code, int var) override {
        switch (code) {
            case SET_ALL:
                this->write(0);
                break;
            case RESET_ALL:
                this->write(255);
                break;
            default:
                break;
        }
        return 1;
    }
    int available() override { return 0; }

    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override { return EOF; }
    void flush() override { return; };
    size_t write(const uint8_t data) override {
        m_relay->busWrite((int)data);

        return ENOSPC;
    }
    size_t write(const uint8_t* buffer, size_t size) override { return ENOSPC; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }

   private:
    uint32_t m_pins[8] = {30, 29, 28, 27, 26, 25, 24, 23};
    BusOut* m_relay = nullptr;
};