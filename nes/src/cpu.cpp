#include "include/cpu.h"


CPU::CPU(Memory& mem) : mem(mem){};

void CPU::reset()
{
    A = X = Y = 0;
    PC = 0xFFFC;// reset vectors
    SP = 0x0100; // stack starts at 0x0100
    P = 0x00; //clear
}

uint8_t CPU::fetch(){
    return mem.read(PC++);
}

uint16_t CPU::fetch16(){
    uint16_t lo = fetch(); 
    uint16_t hi = fetch();
    return (hi << 8) | lo;
} 

