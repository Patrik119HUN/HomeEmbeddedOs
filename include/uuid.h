#pragma once
#include "kernel/kernel.h"
#include <ECCX08.h>
#include <random>

class UUID {
  public:
    UUID() {
        IFile* random_dev = fileSystem.open("/dev/random");
        std::mt19937_64 generator(random_dev->read());
        std::uniform_int_distribution<uint64_t> distrib;
        id = distrib(generator);
    }
    ~UUID() = default;

    uint64_t getUUID() { return id; }

    operator uint64_t() const { return id; }

  private:
    uint64_t id;
};
