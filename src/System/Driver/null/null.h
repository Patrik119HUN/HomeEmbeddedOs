#ifndef null_h
#define null_h

#include <Arduino.h>

#include "../../FileSystem/IFile.h"
class null : public IFile {
   private:
    uint8_t nullVar;

    char _file_name[5] = "null";

   public:
    null() {
        setTimeout(0);
        nullVar = -1;
    }
    int available() override { return 0; }

    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override { return EOF; }

    void flush() override { return; };

    size_t write(const uint8_t data) {
        this->nullVar = data;
        return 1;
    }
    size_t write(const uint8_t* buffer, size_t size) {
        if (size > 0) {
            nullVar = buffer[size - 1];
        }
        return size;
    }

    int lastByte() { return nullVar; }
    int ioctl(int code, int var) { return 1; }
    char* name() override { return _file_name; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }
};

#endif  // null