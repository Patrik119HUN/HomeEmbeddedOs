#pragma once
#include <file_system_interface.h>
#include "fs_type.h"
struct VolumeInformation {
    const char* root_path_name;
    FSType file_system_type;
    IFileSystem* fs;
};
