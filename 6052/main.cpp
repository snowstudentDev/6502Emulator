#include <cstdint>
#include <cstdio>
#include <vector>
using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
private:
    Byte Data[MAX_MEM]; // Private to encapsulate access
public:
    std::vector<const char*> InitValues[MAX_MEM];

    void init()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            WriteByte(i, 0); // Log initialization to 0
        }
    }

    // Read one byte
    Byte operator[](u32 Address) const
    {
        return Data[Address];
    }

    // Write one byte with logging
    void WriteByte(u32 Address, Byte Value)
    {
        Byte old = Data[Address];
        Data[Address] = Value;
		if(Value != 0){
        	printf("Memory at 0x%04X changed from 0x%02X to 0x%02X\n", Address, old, Value);}
    }

    void WriteWord(Word Value, u32 Address, u32& Cycles)
    {
        WriteByte(Address, Value & 0xFF);    // Low byte
        WriteByte(Address + 1, Value >> 8);  // High byte
        Cycles -= 2;
    }
};

struct CPU
{
    Word PC, SP;
    Byte A, X, Y;
    Byte C : 1, Z : 1, I : 1, D : 1, B : 1, V : 1, N : 1;

    void Reset(Mem& memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.init();
    }

    Byte FetchByte(u32& Cycles, Mem& memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    Word FetchWord(u32& Cycles, Mem& memory)
    {
        Word Data = memory[PC];
        PC++;
        Data |= (memory[PC] << 8);
        PC++;
        Cycles -= 2;
        return Data;
    }

    Byte ReadByte(u32& Cycles, Byte Address, Mem& memory)
    {
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    void LDASetStatus()
    {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    void Execute(u32 Cycles, Mem& memory)
    {
        while (Cycles > 0)
        {
            Byte Ins = FetchByte(Cycles, memory);
            switch (Ins)
            {
            case INS_LDA_IM:
            {
                Byte Value = FetchByte(Cycles, memory);
                A = Value;
                LDASetStatus();
            } break;
            case INS_LDA_ZP:
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                A = ReadByte(Cycles, ZeroPageAddress, memory);
                LDASetStatus();
            } break;
            case INS_LDA_ZPX:
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                ZeroPageAddress += X;
                Cycles--;
                A = ReadByte(Cycles, ZeroPageAddress, memory);
                LDASetStatus();
            } break;
            case INS_JSR:
            {
                Word SubAddr = FetchWord(Cycles, memory);
                memory.WriteWord(PC - 1, SP, Cycles);
                SP += 2;
                PC = SubAddr;
                Cycles--;
            } break;
            default:
                printf("Instruction not handled: %d\n", Ins);
                break;
            }
        }
    }

    static constexpr Byte
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_JSR = 0x20;
};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);

    // Program setup using WriteByte to log changes
    mem.WriteByte(0xFFFC, CPU::INS_JSR);
    mem.WriteByte(0xFFFD, 0x42);
    mem.WriteByte(0xFFFE, 0x42);
    mem.WriteByte(0x4242, CPU::INS_LDA_IM);
    mem.WriteByte(0x4243, 0x84);

    cpu.Execute(9, mem);

    return 0;
}