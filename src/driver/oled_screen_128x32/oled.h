#pragma once
#include <Arduino.h>
#include <ArduinoECCX08.h>
#include <Streaming.h>
#include <file_interface.h>
class oled : public IFile {
  public:
    oled() { setTimeout(0); }

    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override { return EOF}

    void flush() override { return; };

    size_t write(const uint8_t data) { return 0; }
    size_t write(const uint8_t* buffer, size_t size) { return 0; }

    int ioctl(int code, int var) { return 1; }
    char* name() override { return m_file_name; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }

  private:
    char m_file_name[5] = "oled";
};