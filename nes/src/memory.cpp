#include "include/memory.h"


Memory::Memory()
{
    for(uint16_t i = 0; i < MAX_MEMORY; i++){
        data[i] = 0;
    }

}


uint8_t Memory::read(uint16_t address) const{
    return data[address];
}

void Memory::write(uint16_t address, uint8_t value)
{
    data[address] = value;
}