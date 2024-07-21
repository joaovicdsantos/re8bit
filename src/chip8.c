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

    for (short i = 0; i < KEY_SIZE; i++)
        chip8->keys[i] = 0x00;
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
        case 0x00EE:
            chip8->SP -= 1;
            chip8->PC = (short)chip8->stack[chip8->SP];
            chip8->stack[chip8->SP] = 0;
            break;
        default:
            printf("Invalid instruction. Opcode: %04X\n", chip8->instruction.op);
            exit(1);
        }
        break;
    case 0x1000:
        chip8->PC = (short)chip8->instruction.nnn;
        break;
    case 0x2000:
        chip8->stack[chip8->SP] = chip8->PC;
        chip8->SP += 1;
        chip8->PC = (short)chip8->instruction.nnn;
        break;
    case 0x3000:
        if (chip8->registers[chip8->instruction.x] == chip8->instruction.nn)
            chip8->PC += 2;
        break;
    case 0x4000:
        if (chip8->registers[chip8->instruction.x] != chip8->instruction.nn)
            chip8->PC += 2;
        break;
    case 0x5000:
        if (chip8->registers[chip8->instruction.x] == chip8->registers[chip8->instruction.y])
            chip8->PC += 2;
        break;
    case 0x6000:
        chip8->registers[chip8->instruction.x] = chip8->instruction.nn;
        break;
    case 0x7000:
        chip8->registers[chip8->instruction.x] += chip8->instruction.nn;
        break;
    case 0x8000:
        switch (chip8->instruction.op & 0x000F)
        {
        case 0x0000:
            chip8->registers[chip8->instruction.x] = chip8->registers[chip8->instruction.y];
            break;
        case 0x0001:
            chip8->registers[chip8->instruction.x] |= chip8->registers[chip8->instruction.y];
            break;
        case 0x0002:
            chip8->registers[chip8->instruction.x] &= chip8->registers[chip8->instruction.y];
            break;
        case 0x0003:
            chip8->registers[chip8->instruction.x] ^= chip8->registers[chip8->instruction.y];
            break;
        case 0x0004:
            short reg_sum = chip8->registers[chip8->instruction.x] + chip8->registers[chip8->instruction.y];
            if (reg_sum > 255)
                chip8->registers[0xF] = 1;
            else
                chip8->registers[0xF] = 0;
            chip8->registers[chip8->instruction.x] = reg_sum & 0xFF;
            break;
        case 0x0005:
            short reg_sub = chip8->registers[chip8->instruction.x] - chip8->registers[chip8->instruction.y];
            if (chip8->registers[chip8->instruction.x] > chip8->registers[chip8->instruction.y])
                chip8->registers[0xF] = 1;
            else
                chip8->registers[0xF] = 0;
            chip8->registers[chip8->instruction.x] = reg_sub;
            break;
        case 0x0006:
            // chip8->registers[chip8->instruction.x] = chip8->registers[chip8->instruction.y];
            chip8->registers[0xF] = chip8->registers[chip8->instruction.x] & 1;
            chip8->registers[chip8->instruction.x] >>= 1;
            break;
        case 0x000E:
            // chip8->registers[chip8->instruction.x] = chip8->registers[chip8->instruction.y];
            chip8->registers[0xF] = chip8->registers[chip8->instruction.x] & 0x80;
            chip8->registers[chip8->instruction.x] <<= 1;
            break;
        default:
            printf("Invalid instruction. Opcode: %04X\n", chip8->instruction.op);
            exit(1);
            break;
        }
        break;
    case 0x9000:
        if (chip8->registers[chip8->instruction.x] != chip8->registers[chip8->instruction.y])
            chip8->PC += 2;
        break;
    case 0xA000:
        chip8->I = chip8->instruction.nnn;
        break;
    case 0xC000:
        chip8->registers[chip8->instruction.x] = (rand() % 256) & chip8->instruction.nn;
        break;
    case 0xD000:
        unsigned char x = chip8->registers[chip8->instruction.x] % 64;
        unsigned char y = chip8->registers[chip8->instruction.y] % 32;
        unsigned short N = chip8->instruction.n;
        unsigned short pixel;
        unsigned char display_overflow = 0;

        chip8->registers[0xF] = 0x00;

        for (short y_line = 0; y_line < N; y_line++)
        {
            pixel = chip8->memory[chip8->I + y_line];
            for (short x_line = 0; x_line < 8; x_line++)
            {
                short pos = (y + y_line) * 64 + x + x_line;
                if (pos > (DISPLAY_SIZE - 1))
                {
                    display_overflow = 1;
                    break;
                }

                if ((pixel >> (7 - x_line) & 1) != 0)
                {
                    if (chip8->display[pos] == 1)
                        chip8->registers[0xF] = 1;
                    chip8->display[pos] ^= 1;
                }
            }
            if (display_overflow == 1)
                break;
        }
        break;
    case 0xE000:
        switch (chip8->instruction.op & 0x00FF)
        {
        case 0x009E:
            if (chip8->keys[chip8->registers[chip8->instruction.x]] == 1)
                chip8->PC += 2;
            break;
        case 0x0A1:
            if (chip8->keys[chip8->registers[chip8->instruction.x]] == 0)
                chip8->PC += 2;
            break;
        default:
            exit(1);
            break;
        }
        break;
    case 0xF000:
        switch (chip8->instruction.op & 0x00FF)
        {
        case 0x007:
            chip8->registers[chip8->instruction.x] = chip8->delay_timer;
            break;
        case 0x00A:
            chip8->PC -= 2;
            for (short i = 0; i < KEY_SIZE; i++)
            {
                if (chip8->keys[i] == 1)
                {
                    chip8->registers[chip8->instruction.x] = i;
                    chip8->PC += 2;
                    break;
                }
            }
            break;
        case 0x015:
            chip8->delay_timer = chip8->registers[chip8->instruction.x];
            break;
        case 0x018:
            chip8->sound_timer = chip8->registers[chip8->instruction.x];
            break;
        case 0x01E:
            chip8->I += chip8->registers[chip8->instruction.x];
            break;
        case 0x029:
            chip8->I = MEMORY_FONT_START + chip8->registers[chip8->instruction.x] * 5;
            break;
        case 0x033:
            chip8->memory[chip8->I] = chip8->registers[chip8->instruction.x] / 100;
            chip8->memory[chip8->I + 1] = (chip8->registers[chip8->instruction.x] / 10) % 10;
            chip8->memory[chip8->I + 2] = (chip8->registers[chip8->instruction.x] % 100) % 10;
            break;
        case 0x055:
            for (short i = 0; i <= chip8->instruction.x; i++)
                chip8->memory[chip8->I + i] = chip8->registers[i];
            break;
        case 0x065:
            for (short i = 0; i <= chip8->instruction.x; i++)
                chip8->registers[i] = chip8->memory[chip8->I + i];
            break;
        default:
            printf("Invalid instruction. Opcode: %04X\n", chip8->instruction.op);
            exit(1);
        }
        break;
    default:
        printf("Invalid instruction. Opcode: %04X\n", chip8->instruction.op);
        exit(1);
    }

    if (chip8->delay_timer > 0)
        chip8->delay_timer--;

    if (chip8->sound_timer > 0)
        chip8->sound_timer--;
}

void reset_keys(Chip8 *chip8)
{
    for (short i = 0; i < KEY_SIZE; i++)
        chip8->keys[i] = 0;
}
