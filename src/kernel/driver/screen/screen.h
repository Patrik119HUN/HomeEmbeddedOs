#ifndef Screen_h
#define Screen_h
#include <Arduino.h>
#include <LiquidCrystal.h>

#include "../../file_system/file_interface.h"
constexpr uint8_t LCD_COLS = 20;
constexpr uint8_t LCD_ROWS = 4;

constexpr uint8_t rs = PC8;
constexpr uint8_t rw = PC6;
constexpr uint8_t en = PC5;
constexpr uint8_t d0 = PA12;
constexpr uint8_t d1 = PA11;
constexpr uint8_t d2 = PB12;
constexpr uint8_t d3 = PB11;
constexpr uint8_t d4 = PB2;
constexpr uint8_t d5 = PB1;
constexpr uint8_t d6 = PB15;
constexpr uint8_t d7 = PB14;
class Screen : public IFile {
   private:
    LiquidCrystal* LCDinstance = NULL;
    char _file_name[4] = "scr";

   public:
    enum CTRLCMD {
        CLEAR,
        HOME,
        SET_CURSOR,
        DISPLAY_CURSOR,
        BLINK_CURSOR,
        SET_DISPLAY,
        SET_AUTOSCROLL
    };
    Screen() {
        this->LCDinstance = new LiquidCrystal(rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);
        this->LCDinstance->begin(LCD_COLS, LCD_ROWS);
    };
    bool seek(uint32_t pos) override {
        int col = pos % LCD_COLS;
        int row = pos / LCD_ROWS;
        if (pos < 0 || pos > 80) return false;
        this->LCDinstance->setCursor(col, row);
        return true;
    }

    size_t write(const uint8_t data) override {
        this->LCDinstance->write(data);
        return 1;
    };
    int ioctl(int code, int var) override {
        switch (code) {
            case CLEAR:
                this->LCDinstance->clear();
                break;
            case HOME:
                this->LCDinstance->home();
                break;
            case SET_CURSOR:
                this->seek(var);
                break;
            case DISPLAY_CURSOR:
                if (var) {
                    this->LCDinstance->cursor();
                } else {
                    this->LCDinstance->noCursor();
                }
                break;
            case BLINK_CURSOR:
                if (var) {
                    this->LCDinstance->blink();
                } else {
                    this->LCDinstance->noBlink();
                }
                break;
            case SET_DISPLAY:
                if (var) {
                    this->LCDinstance->display();
                } else {
                    this->LCDinstance->noDisplay();
                }
                break;
            case SET_AUTOSCROLL:
                if (var) {
                    this->LCDinstance->autoscroll();
                } else {
                    this->LCDinstance->noAutoscroll();
                }
                break;
            default:
                return 0;
                break;
        }
        return 1;
    }
    int available() override { return 0; }
    int peek() override { return EOF; }
    int read() override { return 0; }
    void flush() override { return; };

    size_t write(const uint8_t* buffer, size_t size) {
        for (size_t i = 0; i < size; i++) {
            this->LCDinstance->write(buffer[i]);
        }
        return 1;
    }
    char* name() override { return _file_name; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }
};

#endif  // Screen