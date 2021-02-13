#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct registers{
    uint16_t PC;
    uint16_t I;
    uint8_t SP;
    uint8_t v[16];
} REGISTERS;

// all instructions are 2 bytes so grab 2 bytes from RAM and bump PC by 2
uint16_t fetch(REGISTERS* registers, uint8_t* ram){
    uint8_t first_byte = ram[registers->PC];
    registers->PC++;
    uint8_t second_byte = ram[registers->PC];
    registers->PC++;
    uint16_t val = (first_byte << 8) | second_byte;
    return val;
}

void execute_0(uint16_t instruction, REGISTERS* registers, uint8_t* ram){
    switch(instruction){
        case 0x00E0:
            // Clear screen
            break;
        case 0x00EE:
            // return from subrouting
            break;
    }
}

void execute_1(uint16_t nnn, REGISTERS* registers){
    // JUMP - set PC to nnn
    registers->PC = nnn;
}

void execute_2(uint16_t nnn, REGISTERS* registers, uint8_t* ram){
}

void execute_3(uint8_t x, uint8_t kk, REGISTERS* registers){
}

void execute_4(uint8_t x, uint8_t kk, REGISTERS* registers){
}

void execute_5(uint8_t x, uint8_t y, REGISTERS* registers){
}

void execute_6(uint8_t x, uint8_t kk, REGISTERS* registers){
    // SET register x to kk
    registers->v[x] = kk;
}

void execute_7(uint8_t x, uint8_t kk, REGISTERS* registers){
    // ADD kk to current value of v[x]
    registers->v[x] = registers->v[x] + kk;
}

void execute_8(uint8_t x, uint8_t y, REGISTERS* registers){
}

void execute_9(uint8_t x, uint8_t y, REGISTERS* registers){
}

void execute_a(uint16_t nnn, REGISTERS* registers){
    // SET I to nnn
    registers->I = nnn;
}

void execute_b(uint16_t nnn, REGISTERS* registers, uint8_t* ram){
}

void execute_c(uint8_t x, uint8_t kk, REGISTERS* registers){
}

void execute_d(uint8_t x, uint8_t y, REGISTERS* registers, uint8_t* ram){
}

void execute_e(uint8_t x, REGISTERS* registers){
}

void execute_f(uint8_t x, REGISTERS* registers, uint8_t* ram){
}

void decode_and_execute(uint16_t instruction, REGISTERS* registers, uint8_t* ram){
    uint8_t top_nibble = instruction >> 12;
    uint16_t nnn = instruction & 0x0FFF;
    uint8_t kk = instruction & 0x00FF;
    uint8_t x = (instruction & 0x0F00) >> 8;
    uint8_t y = (instruction & 0x00F0) >> 4;

    printf("top nibble : %02X\n", top_nibble);
    switch(top_nibble){
        case 0x0: // 00E0 00EE (0nnn unused)
            execute_0(instruction, registers, ram);
            break;
        case 0x1: // 1nnn
            execute_1(nnn, registers);
            break;
        case 0x2: // 2nnn
            execute_2(nnn, registers, ram);
            break;
        case 0x3: // 3xkk
            execute_3(x, kk, registers);
            break;
        case 0x4: // 4xkk
            execute_4(x, kk, registers);
            break;
        case 0x5: // 5xy0
            execute_5(x, y, registers);
            break;
        case 0x6: // 6xkk
            execute_6(x, kk, registers);
            break;
        case 0x7: // 7xkk
            execute_7(x, kk, registers);
            break;
        case 0x8: // 8xy0 8xy1 8xy2 8xy3 8xy4 8xy5 8xy6 8xy7 8xyE
            execute_8(x, y, registers);
            break;
        case 0x9: // 9xy0
            execute_9(x, y, registers);
            break;
        case 0xA: // Annn
            execute_a(nnn, registers);
            break;
        case 0xB: // Bnnn
            execute_b(nnn, registers, ram);
            break;
        case 0xC: // Cxkk
            execute_c(x, kk, registers);
            break;
        case 0xD: // Dxyn
            execute_d(x, y, registers, ram);
            break;
        case 0xE: // Ex9E ExA1
            execute_e(x, registers);
            break;
        case 0xF: // Fx07 Fx0A Fx15 Fx18 Fx1E Fx29 Fx33 Fx55 Fx65
            execute_f(x, registers, ram);
            break;
    }
}

void init_registers(REGISTERS* registers){
    memset(registers->v, 0, 16);
    registers->I = 0;
    registers->PC = 0;
    registers->SP = 0;
}

// Implement 00E0, 1NNN, 6XNN, 7XNN, ANNN, DXYN for IBM logo test
int main(int argc, char* argv[]){
    REGISTERS registers;
    uint8_t ram[4096] = { 0 };

    init_registers(&registers);

    //put test value in RAM
    ram[0x1F1] = 0xBE;
    ram[0x1F2] = 0xEF;

    registers.PC = 0X01F1;
    printf("PC Before : %04X\n", registers.PC);

    uint16_t fetched = fetch(&registers, ram);
    printf("fetched : %04X\n", fetched);
    printf("PC After : %04X\n", registers.PC);

    decode_and_execute(0xFABE, &registers, ram);

    printf("v[2] before : %02X\n", registers.v[0x02]);
    execute_7(0x02, 0x22, &registers);
    printf("v[2] after : %02X\n", registers.v[0x02]);

}