#pragma once
#include <string>

class IDevice {
   public:
    IDevice() {}
    IDevice(std::string name) : m_name{name} {}
    virtual ~IDevice() {}
    std::string get_name() { return m_name; }

   protected:
    std::string m_name;
};