#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>

#include "const.h"
#include <file_interface.h>
class Screen : public IFile {

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
        m_name="Liquid_Crystal_Screen";
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
    bool isDirectory(void) override { return false; }
    void close() override { return; }

  private:
    LiquidCrystal* LCDinstance = NULL;
};