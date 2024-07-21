#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>

const unsigned char FONTS[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void _init_(Chip8 *chip8)
{
    chip8->I = 0;
    chip8->PC = 0x200;
    chip8->SP = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    for (short i = 0; i < MEMORY_SIZE; i++)
    {
        chip8->memory[i] = 0x00;
        if (i >= MEMORY_FONT_START && i < MEMORY_FONT_END) // Load fonts
            chip8->memory[i] = FONTS[i - MEMORY_FONT_START];
    }

    for (short i = 0; i < DISPLAY_SIZE; i++)
        chip8->display[i] = 0x00;

    for (short i = 0; i < REGISTER_SIZE; i++)
        chip8->registers[i] = 0x00;

    for (short i = 0; i < STACK_SIZE; i++)
        chip8->stack[i] = 0x00;
}

Chip8 new_chip8()
{
    Chip8 chip8;

    _init_(&chip8);

    return chip8;
}

void load_rom(Chip8 *chip8, const char *rom_path)
{
    FILE *rom = fopen(rom_path, "rb");
    if (!rom)
    {
        printf("Failed to open ROM.\n");
        exit(1);
    }
    fread(chip8->memory + 0x200, 1, 4096, rom);
    fclose(rom);
}

void _fetch(Chip8 *chip8)
{
    chip8->instruction.op = (chip8->memory[chip8->PC] << 8) | chip8->memory[chip8->PC + 1];
    chip8->PC += 2;
}

void _decode(Chip8 *chip8)
{
    chip8->instruction.x = (chip8->instruction.op & 0x0F00) >> 8;
    chip8->instruction.y = (chip8->instruction.op & 0x00F0) >> 4;
    chip8->instruction.n = chip8->instruction.op & 0X000F;
    chip8->instruction.nn = chip8->instruction.op & 0X00FF;
    chip8->instruction.nnn = chip8->instruction.op & 0X0FFF;
}

void emulate_cycle(Chip8 *chip8)
{
    _fetch(chip8);
    _decode(chip8);

    switch (chip8->instruction.op & 0xF000)
    {
    case 0x0000:
        switch (chip8->instruction.op & 0x00FF)
        {
        case 0x00E0:
            for (short i = 0; i < DISPLAY_SIZE; i++)
                chip8->display[i] = 0;
            break;
        default:
            printf("Invalid instruction. Opcode: %x\n", chip8->instruction.op);
        }
        break;
    case 0x1000:
        chip8->PC = chip8->instruction.nnn;
        break;
    case 0x6000:
        chip8->registers[chip8->instruction.x] = chip8->instruction.nn;
        break;
    case 0x7000:
        chip8->registers[chip8->instruction.x] += chip8->instruction.nn;
        break;
    case 0xA000:
        chip8->I = chip8->instruction.nnn;
        break;
    case 0xD000:
        unsigned char x = chip8->registers[chip8->instruction.x] % 64;
        unsigned char y = chip8->registers[chip8->instruction.y] % 32;
        unsigned short N = chip8->instruction.n;
        unsigned short pixel;

        chip8->registers[0xF] = 0x00;
        for (short y_line = 0; y_line < N; y_line++)
        {
            pixel = chip8->memory[chip8->I + y_line];
            for (short x_line = 0; x_line < 8; x_line++)
            {
                if ((pixel >> (7 - x_line) & 1) != 0)
                {
                    if (chip8->display[(y + y_line) * 64 + x + x_line] == 1)
                        chip8->registers[0xF] = 1;
                    chip8->display[(y + y_line) * 64 + x + x_line] ^= 1;
                }
            }
        }
        break;
    default:
        printf("Invalid instruction. Opcode: %x\n", chip8->instruction.op);
    }

    if (chip8->delay_timer > 0)
        chip8->delay_timer--;

    if (chip8->sound_timer > 0)
        chip8->sound_timer--;
}
