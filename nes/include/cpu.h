#ifndef CPU_H
#define CPU_H


#include "memory.h"
#include <cstdint>

class CPU
{
public:
    CPU(Memory& mem);
    void reset();
    void execute();


private:

//regiseters
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint16_t PC;
    uint8_t SP;
    uint8_t P;


    Memory& mem;

    uint8_t fetch();
    uint16_t fetch16();
    void push(uint8_t value);
    uint8_t pop();
    
};


#endif