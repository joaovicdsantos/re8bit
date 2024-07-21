#ifndef CHIP8_H_
#define CHIP8_H_

// General Definitions
#define MEMORY_SIZE 4096
#define DISPLAY_SIZE 64 * 32
#define REGISTER_SIZE 16
#define STACK_SIZE 32
#define KEY_SIZE 16

#define MEMORY_FONT_START 0x50
#define MEMORY_FONT_END 0x9F

typedef struct instruction
{
    unsigned short op;
    unsigned char x;
    unsigned char y;
    unsigned char n;
    unsigned char nn;
    unsigned short nnn;
} Instruction;

typedef struct chip8
{
    unsigned char memory[MEMORY_SIZE];
    unsigned char display[DISPLAY_SIZE];
    unsigned short PC;
    unsigned short I;
    unsigned char registers[REGISTER_SIZE];
    unsigned short stack[STACK_SIZE];
    unsigned char SP;
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short keys[KEY_SIZE];
    Instruction instruction;
} Chip8;

Chip8 new_chip8();
void load_rom(Chip8 *chip8, const char *rom_path);
void emulate_cycle(Chip8 *chip8);
void reset_keys(Chip8 *chip8);

#endif
