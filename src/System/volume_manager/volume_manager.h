#ifndef VolumeManager_h
#define VolumeManager_h

#include <Arduino.h>
#include <Streaming.h>

#include <vector>

#include "../FileSystem/IFileSystem.h"
#include "fs_type.h"
#include "volume_information.h"

class VolumeManager {
   public:
    VolumeManager(const VolumeManager&) = delete;
    VolumeManager& operator=(const VolumeManager&) = delete;
    static VolumeManager* get_instance();

    void Mount(const char* name, FSType type, IFileSystem* fsptr);

    void UnMount(const char* name);

    IFileSystem* get_volume(const char* name);

   private:
    inline static VolumeManager* instance{nullptr};

    std::vector<VolumeInformation*> volumes_;

    VolumeInformation* Search(const char* name);

    VolumeManager() = default;
    ~VolumeManager() = default;
};

#endif  // VolumeManager