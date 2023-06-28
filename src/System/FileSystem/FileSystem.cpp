//
// Created by patri on 2023. 06. 25..
//

#include "FileSystem.h"

node* FileSystem::search(const char* name, nodeType type, node* actualFolder) {
    for (auto i : actualFolder->files) {
        if (i->type == type && strcmp(i->name, name) == 0) {
            return i;
        }
    }
    return nullptr;
}

int FileSystem::mkdir(const char* path) {
    if (strlen(path) <= 0) return ENOENT;
    if (strlen(path) > PATH__MAX) return ENAMETOOLONG;

    std::vector<char*> paths = tokenize(path, "/");
    std::reverse(paths.begin(), paths.end());

    if (paths.size() > SYMLOOP_MAX) return ELOOP;
    for (auto i : paths)
        if (strlen(i) > NAME_MAX) return ENAMETOOLONG;
    node* lastInstance = &root;

    while (!paths.empty()) {
        node* srchres = this->search(paths.back(), FOLDER, lastInstance);
        if (srchres != nullptr) {
            lastInstance = srchres;
        } else {
            node* newFolder = new node;
            newFolder->name = paths.back();
            lastInstance->files.push_back(newFolder);
            lastInstance = newFolder;
        }
        paths.pop_back();
    }
    return 1;
}

int FileSystem::rmdir(const char* path) {
    std::vector<char*> paths = tokenize(path, "/");
    std::reverse(paths.begin(), paths.end());

    node* folder;
    node* lastInstance = &root;

    while (paths.size() != 1) {
        node* srchres = this->search(paths.back(), FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.pop_back();
    }

    folder = this->search(paths.back(), FOLDER, lastInstance);
    paths.pop_back();
    if (!folder->files.empty()) return 0;
    auto it = std::find(lastInstance->files.begin(), lastInstance->files.end(), folder);
    lastInstance->files.erase(it);
    return 1;
}

void FileSystem::rmnod(const char* path) {}

int FileSystem::mknod(const char* path, node* file) {
    node* lastInstance = &root;
    std::vector<char*> paths = tokenize(path, "/");
    std::reverse(paths.begin(), paths.end());
    file->name = paths.front();
    paths.erase(paths.begin());

    while (!paths.empty()) {
        node* srchres = this->search(paths.back(), FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.pop_back();
    }
    if (this->search(file->name, file->type, lastInstance) != nullptr) return 0;
    lastInstance->files.push_back(file);
    return 1;
}

int FileSystem::mknod(const char* path, dev_t dev) {
    node* file = new node;
    file->type = DEVICE;
    file->dev = dev;
    return this->mknod(path, file);
}

int FileSystem::mknod(const char* path) {
    node* file = new node;
    return this->mknod(path, file);
}

Device* FileSystem::open(const char* path) {
    node* lastInstance = &root;
    Device* dev;
    std::vector<char*> paths = tokenize(path, "/");
    std::reverse(paths.begin(), paths.end());
    char* devName = paths.front();
    paths.erase(paths.begin());

    while (!paths.empty()) {
        node* srchres = this->search(paths.back(), FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.pop_back();
    }
    node* devFile = this->search(devName, DEVICE, lastInstance);
    if (devFile == nullptr) return 0;
    dev = dmInstance->getDev(devFile->dev);
    return dev;
}