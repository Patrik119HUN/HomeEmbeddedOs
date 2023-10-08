#pragma once
#include <Arduino.h>


#include <map>
#include <vector>

#include <sys/macros.h>
#include "node.h"
#define PATH__MAX 64
#define NAME_MAX_ 32
#define SYMLOOP_MAX 10
#include <file_system_interface.h>

using namespace node;
class FileSystem : public IFileSystem {
  public:
    FileSystem() {
        m_root.type = node::Type::FOLDER;
        m_root.name = "";
    };
    ~FileSystem() = default;

#pragma region Public
  public:
    String currentPath(Node* t_actualFolder);
    Node* search(const char* t_name, Type t_type, Node* t_actualFolder);
    uint8_t mknod(const char* t_path);

    uint8_t mknod(const char* t_path, int t_dev);

    void rmnod(const char* t_path);

    uint8_t rmdir(const char* t_path) override;
    uint8_t rmdir(const String& t_path) override{ return this->rmdir(t_path.c_str()); };

    bool remove(const char* t_path) override{ return true; }
    bool remove(const String& t_path) override { return this->remove(t_path.c_str()); };

    bool exists(const char* t_path) override { return true; }
    bool exists(const String& t_path) override { return this->exists(t_path.c_str()); };

    uint8_t mkdir(const char* t_path) override;
    uint8_t mkdir(const String& t_path) override{ return this->mkdir(t_path.c_str()); };
    IFile* open(const char* t_path, uint8_t t_mode = 1) override;
    IFile* open(const String& t_path, uint8_t t_mode = 1) override {
        return this->open(t_path.c_str(), t_mode);
    };
    Node* lastFilePointer = &m_root;
#pragma endregion

#pragma region Private
  private:
    uint8_t mknod(const char* t_path, Node* t_node);
    Node m_root;
};
#pragma endregion
extern FileSystem fileSystem;