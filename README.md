# Re8Bit

https://github.com/user-attachments/assets/8e03cde8-b82c-442c-99d2-d4299b221fa0

A simple Chip-8 emulator/interpreter made in C using SFML (or CSML, precisely).


## Usage

To run a ROM everything you need is to execute the following command:

```bash
$ ./bin/re8bit <rom>
```

You can find ROMs in this repository: https://github.com/kripod/chip8-roms

## Setup & Build

You must install some dependencies to build `Re8Bit` on your machine. Make to be able to run the commands in the Makefile and [CSFML](https://www.sfml-dev.org/tutorials/2.6/start-linux.php#installing-sfml), a C bind to SFML which will provide all necessary libs to Graphics and Audio.
After the dependencies installation, just run:

```bash
$ make
```
