#pragma once
#include <Arduino.h>

typedef int (*parametered_function)(int, char**);
class ProcessWrapper {
  public:
    ProcessWrapper(parametered_function t_func) { this->m_func = t_func; }
    void setArgc(int t_num) { this->m_argc = t_num; }
    void setArgv(char** t_argv) { this->m_argv = t_argv; }
    int getReturn() { return m_ret; }
    void func() { m_ret = m_func(m_argc, m_argv); }

  private:
    char** m_argv;
    int m_argc, m_ret;
    parametered_function m_func;
};