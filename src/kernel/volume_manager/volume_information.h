#ifndef volume_information_h
#define volume_information_h

struct VolumeInformation {
    const char* root_path_name;
    FSType file_system_type;
    IFileSystem* fs;
    void print() { Serial << root_path_name << " " << file_system_type << endl; }
};

#endif  // volume_information