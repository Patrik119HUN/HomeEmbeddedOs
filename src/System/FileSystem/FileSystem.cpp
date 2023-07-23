//
// Created by patri on 2023. 06. 25..
//

#include "FileSystem.h"

node* FileSystem::search(const char* name, nodeType type, node* actualFolder) {
    std::vector<node*> vec = actualFolder->files;
    for (auto i : vec) {
        if (i->type == type && strcmp(i->name, name) == 0) {
            return i;
        }
    }
    return nullptr;
}

uint8_t FileSystem::mkdir(const char* path) {
    if (strlen(path) <= 0) return ENOENT;
    if (strlen(path) > PATH__MAX) return ENAMETOOLONG;
    char* _path = strdup(path);
    std::vector<char*> paths = tokenize(_path, "/");

    if (paths.size() > SYMLOOP_MAX) return ELOOP;
    for (auto i : paths)
        if (strlen(i) > NAME_MAX_) return ENAMETOOLONG;

    node* lastInstance = (_path[0] == '/') ? &root : lastFilePointer;

    while (!paths.empty()) {
        node* srchres = this->search(paths.front(), FOLDER, lastInstance);
        if (srchres != nullptr) {
            lastInstance = srchres;
        } else {
            node* newFolder = new node;
            newFolder->name = paths.front();
            newFolder->prevNode = lastInstance;
            lastInstance->files.push_back(newFolder);
            lastInstance = newFolder;
        }
        paths.erase(paths.begin());
    }
    return 1;
}

uint8_t FileSystem::rmdir(const char* path) {
    std::vector<char*> paths = tokenize(path, "/");

    node* folder;
    char* _path = strdup(path);
    node* lastInstance = (_path[0] == '/') ? &root : lastFilePointer;

    while (paths.size() != 1) {
        node* srchres = this->search(paths.front(), FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.erase(paths.begin());
    }

    folder = this->search(paths.front(), FOLDER, lastInstance);
    paths.erase(paths.begin());
    if (!folder->files.empty()) return 0;
    auto it = std::find(lastInstance->files.begin(), lastInstance->files.end(), folder);
    lastInstance->files.erase(it);
    return 1;
}

void FileSystem::rmnod(const char* path) {}


uint8_t FileSystem::mknod(const char* path, node* file) {
    char* _path = strdup(path);
    node* lastInstance = (_path[0] == '/') ? &root : lastFilePointer;
    std::vector<char*> paths = tokenize(path, "/");
    file->name = paths.back();
    paths.pop_back();

    while (!paths.empty()) {
        node* srchres = this->search(paths.front(), FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.erase(paths.begin());
    }
    if (this->search(file->name, file->type, lastInstance) != nullptr) return 0;
    lastInstance->files.push_back(file);
    return 1;
}

String FileSystem::currentPath(node* actualFolder) {
    String path = "";
    std::vector<String> pathNames;
    node* lastInstance = actualFolder;
    pathNames.push_back(lastInstance->name);
    while (true) {
        if (lastInstance->prevNode == nullptr) break;
        lastInstance = lastInstance->prevNode;
        pathNames.push_back(lastInstance->name);
    }
    while (!pathNames.empty()) {
        path += pathNames.back();
        pathNames.pop_back();
        path += "/";
    }
    return path;
}

uint8_t FileSystem::mknod(const char* path, dev_t dev) {
    node* file = new node;
    file->type = DEVICE;
    file->dev = dev;
    return this->mknod(path, file);
}

uint8_t FileSystem::mknod(const char* path) {
    node* file = new node;
    return this->mknod(path, file);
}

IFile* FileSystem::open(const char* path,uint8_t mode) {
    char* _path = strdup(path);
    node* lastInstance = (_path[0] == '/') ? &root : lastFilePointer;
    IFile* dev;
    std::vector<char*> paths = tokenize(path, "/");
    char* devName = paths.back();
    paths.pop_back();

    while (!paths.empty()) {
        node* srchres = this->search(paths.front(), FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.erase(paths.begin());
    }
    node* devFile = this->search(devName, DEVICE, lastInstance);
    if (devFile == nullptr) return nullptr;
    dev = dmInstance->getDev(devFile->dev);
    return dev;
}