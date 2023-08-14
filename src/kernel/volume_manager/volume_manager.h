#pragma once
#include <Arduino.h>
#include <Streaming.h>

#include <vector>

#include "fs_type.h"
#include "volume_information.h"
#include <file_system_interface.h>

class VolumeManager {
  public:
    VolumeManager() = default;
    ~VolumeManager() = default;

  public:
    void mount(const char* t_name, FSType t_type, IFileSystem* t_fsptr);

    void unMount(const char* t_name);

    IFileSystem* getVolume(const char* t_name);

  private:
    std::vector<VolumeInformation*> m_volumes;

    VolumeInformation* search(const char* t_name);
};
extern VolumeManager volumeManager;
