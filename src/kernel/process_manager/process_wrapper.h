#pragma once
#include "typedef.h"
#include <Arduino.h>
class ProcessWrapper {
  public:
    ProcessWrapper(bool parametered) { this->isParametered = parametered; }

    void setTask(parametered_function t_func) {
        if (isParametered) {
            this->m_param_func = t_func;
        } else {
            Serial.println("Wrong function wrapper call");
        }
    }
    void setTask(basic_function t_func) {
        if (!isParametered) {
            this->m_func = t_func;
        } else {
            Serial.println("Wrong function wrapper call");
        }
    }

    void setArgc(int t_num) { this->m_argc = t_num; }
    void setArgv(char** t_argv) { this->m_argv = t_argv; }
    int getReturn() { return m_ret; }
    void func(void* f) { m_ret = (isParametered) ? m_param_func(m_argc, m_argv) : m_func(); }

  private:
    bool isParametered = false;
    char** m_argv;
    int m_argc, m_ret;
    parametered_function m_param_func;
    basic_function m_func;
};