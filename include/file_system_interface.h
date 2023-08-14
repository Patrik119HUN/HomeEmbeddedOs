#pragma once

#include <Arduino.h>

#include "file_interface.h"
class IFileSystem {
   public:
    virtual IFile* open(const char*, uint8_t) = 0;
    virtual IFile* open(const String&, uint8_t) = 0;

    virtual bool exists(const char*) = 0;
    virtual bool exists(const String&) = 0;

    virtual uint8_t mkdir(const char*) = 0;
    virtual uint8_t mkdir(const String&) = 0;

    virtual uint8_t rmdir(const char*) = 0;
    virtual uint8_t rmdir(const String&) = 0;

    virtual bool remove(const char*) = 0;
    virtual bool remove(const String&) = 0;
};