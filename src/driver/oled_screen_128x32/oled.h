#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <file_interface.h>
#include <memory>

class oled : public IFile {
  public:
    oled() {
        m_display.clearDisplay();
        m_display.setCursor(0, 0);
        m_display.setTextColor(WHITE);
        m_display.setTextSize(1);
        m_display.display();
        setTimeout(0);
    }
    int available() override { return 1; }
    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override { return EOF; }

    void flush() override { m_display.display(); };

    size_t write(const uint8_t data) override { return m_display.write(data); }
    size_t write(const uint8_t* buffer, size_t size) override {
        return m_display.write(buffer, size);
    }

    int ioctl(int code, int var) override {
        switch (code) {
        case 1:
            m_display.clearDisplay();
            return 1;

        default:
            return 0;
        }
    }
    char* name() override { return m_file_name; }
    bool isDirectory(void) override { return false; }
    void close() override { flush(); }

  private:
    char m_file_name[5] = "oled";
};