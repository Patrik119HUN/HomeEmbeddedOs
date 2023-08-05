#ifndef VolumeManager_h
#define VolumeManager_h

#include <Arduino.h>
#include <Streaming.h>

#include <vector>

#include <file_system_interface.h>
#include "fs_type.h"
#include "volume_information.h"

class VolumeManager {
   public:
    VolumeManager(const VolumeManager&) = delete;
    VolumeManager& operator=(const VolumeManager&) = delete;
    static VolumeManager* getInstance();

    void mount(const char* t_name, FSType t_type, IFileSystem* t_fsptr);

    void unMount(const char* t_name);

    IFileSystem* getVolume(const char* t_name);

   private:
    inline static VolumeManager* m_instance{nullptr};

    std::vector<VolumeInformation*> m_volumes;

    VolumeInformation* search(const char* t_name);

    VolumeManager() = default;
    ~VolumeManager() = default;
};

#endif  // VolumeManager