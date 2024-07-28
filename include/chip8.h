#ifndef CHIP8_H_
#define CHIP8_H_

#include <stdint.h>

// General Chip8 Definitions
#define MEMORY_SIZE 4096
#define DISPLAY_SIZE 64 * 32
#define REGISTER_SIZE 16
#define STACK_SIZE 32
#define KEY_SIZE 16
#define MEMORY_FONT_START 0x50
#define MEMORY_FONT_END 0x9F

typedef struct instruction
{
    uint16_t op;
    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint8_t nn;
    uint16_t nnn;
} Instruction;

typedef struct chip8
{
    uint8_t memory[MEMORY_SIZE];
    uint8_t display[DISPLAY_SIZE];
    uint16_t PC;
    uint16_t I;
    uint8_t registers[REGISTER_SIZE];
    uint16_t stack[STACK_SIZE];
    uint8_t SP;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t keys[KEY_SIZE];
    Instruction instruction;
    bool paused;
    bool muted;
} Chip8;

Chip8 new_chip8();
void load_rom(Chip8 *chip8, const char *rom_path);
void decrease_timers(Chip8 *chip8);
void emulate_cycle(Chip8 *chip8);
void reset_keys(Chip8 *chip8);

#endif
