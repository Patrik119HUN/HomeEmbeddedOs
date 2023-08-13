#ifndef random_h
#define random_h

#include <Arduino.h>

#include <file_interface.h>
class random : public IFile {
  private:
    char m_file_name[5] = "rand";
    bool m_blocked = false;
    uint32_t m_rand_val;

  public:
    random() {
        setTimeout(0);
    }
    int available() override { return m_blocked; }

    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override {
        randomSeed(analogRead(A0));
        uint32_t new_val = random(0, UINT32_MAX);
        if (m_rand_val = new_val) m_blocked = true;
        return m_rand_val; 
    }

    void flush() override { return; };

    size_t write(const uint8_t data) { this->nullVar = data; }
    size_t write(const uint8_t* buffer, size_t size) { return size; }

    int ioctl(int code, int var) { return 1; }
    char* name() override { return _file_name; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }
}
#endif // random