#ifndef Screen_h
#define Screen_h
#include <Arduino.h>
#include <LiquidCrystal.h>

#include "../../Utils/Device/Device.h"

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
class Screen : public Device {
   private:
    LiquidCrystal* LCDinstance = NULL;

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
    int seek(int pos) {
        int col = pos % LCD_COLS;
        int row = pos / LCD_ROWS;
        if (pos < 0 || pos > 80) return 0;
        this->LCDinstance->setCursor(col, row);
        return 1;
    }
    int write(int str) {
        this->LCDinstance->print(str);
        return 1;
    };
    int read() { return 1; }

    int ioctl(int code, int var) {
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
};

#endif  // Screen