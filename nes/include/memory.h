#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>

class Memory {
    public:
    static constexpr uint16_t MAX_MEMORY = 0xFFFF;//64kb

    Memory();
    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);

private:
    uint8_t data[MAX_MEMORY];
};


#endif // MEMORY_H