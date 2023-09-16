#pragma once
#include <Arduino.h>
#include <Streaming.h>
#include <map>

#include "../../driver/drivers.h"
class DeviceManager {
  public:
    DeviceManager() = default;
    ~DeviceManager() = default;

  public:
    void addDevice(std::string t_name, IFile* p_dev) {
        m_devices.insert(std::pair{t_name, p_dev});
    }
    IFile* open(std::string t_name) { return m_devices.at(t_name); }

    void removeDevice(std::string t_name) { m_devices.erase(t_name); }

  private:
    std::unordered_map<std::string, IFile*> m_devices;
};
DeviceManager deviceManager;