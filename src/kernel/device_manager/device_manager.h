#pragma once
#include "../../driver/drivers.h"
#include <Arduino.h>
#include <Streaming.h>
#include <map>
using std::string, std::pair;
class DeviceManager {
  public:
    DeviceManager() = default;
    ~DeviceManager() = default;

  public:
    void add_device(string t_name, IFile* p_dev) { m_devices.insert(pair{t_name, p_dev}); }
    IFile* open(string t_name) { return m_devices.at(t_name); }

    void remove_device(string t_name) { m_devices.erase(t_name); }

  private:
    std::unordered_map<string, IFile*> m_devices;
};
DeviceManager deviceManager;