#include "device_manager.h"

DeviceManager* DeviceManager::GetInstance() {
    if (!instance) {
        instance = new DeviceManager();
    }
    return instance;
}

dev_t DeviceManager::AddDevice(DeviceTypes type, IFile* dev) {
    dev_t a = device_record_.at(type);
    dev_t id = makedev(type, a);
    devices_.insert(std::pair{id, dev});
    device_record_[type]++;
    return id;
};

IFile* DeviceManager::GetDevice(dev_t id) { return devices_.at(id); }

uint8_t DeviceManager::RemoveDevice(dev_t id) {
    devices_.erase(id);
    return 1;
}
int DeviceManager::Exists(IFile* devPtr) {
    for (auto i : devices_) {
        if (i.second == devPtr) return 1;
    }
    return 0;
}