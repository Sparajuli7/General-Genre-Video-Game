#pragma once

#include <atomic>

/* Creating a UUID will provide a unique ID by providing a value of counter + 1 and increasing counter by 1 */
class Uuid {
public:
    Uuid() : value(counter++) {};
    Uuid(int uuid) : value(uuid) {};
    const uint64_t value;
    operator int() const { return value; }

private:
    inline static std::atomic<uint32_t> counter = 0;
};