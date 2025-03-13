#pragma once

#include <atomic>

class Uuid {
public:
    Uuid() : value(counter++) {};
    Uuid(uint32_t team) : value(team << 32 + Uuid::counter++) {};
    const uint64_t value;
    operator int() const { return value; }

private:
    inline static std::atomic<uint32_t> counter = 0;
};