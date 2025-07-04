#ifndef CHIP_8_
#define CHIP_8_

#include <iostream>
#include <SDL.h>
#include <fstream>
#include <random>
#include <chrono>

constexpr unsigned int FONTSET_SIZE{ 80 }; //size of fonset
const unsigned int FONTSET_START_ADDRESS = 0x50;
constexpr uint32_t VIDEO_WIDTH{ 64 };
constexpr uint32_t VIDEO_HEIGHT{ 32 };

class Chip8 {
	public:
	Chip8();
	~Chip8();

	uint8_t memory[4096]{}; // Memory (4KB)
	uint8_t registers[16]{}; // 16 8-bit registers (V0-VF)
	uint16_t indexRegister{}; // Index register (I)
	uint8_t delayTimer{}; // Delay timer
	uint8_t soundTimer{}; // Sound timer - nwhen non zero there are deincremented by a reate of 60hz
	uint16_t programCounter{}; // Program counterm - stores the current addres,
	uint16_t stack[16]; // Stack - 16 levels deep


	uint8_t stackPointer{}; // Stack pointer - points to the current top of the stack

	//keyboard - held a 16-key hexiadecimal keepad, layout must be mapped into various other configs to fit keyboard of todays platforms
	uint8_t keypad[16]; // Keypad state (0x0 - 0xF)
	uint32_t display[64 * 32]; // Display (64x32 pixels)

	//sprites, a grpup of bytes which are a binary rep of the desired picture, sprited are up to 15 byted
	unsigned char sprites[15][5]; // Predefined sprites (0-9, A-F, and some others) - should be stores in the interpreter area pf chip 8 mem (0x00 to 0x1ff)

	uint16_t opcode; // Current instruction being executed

	uint8_t fontset[FONTSET_SIZE] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF8, 0x90, 0xF8, // 6
		0xF8, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F


	};

	std::default_random_engine ramdGen; // Random number generator
	std::uniform_int_distribution<uint8_t> randByte; // Random byte generator for sound and delay timers

	void initialize();
	void loadProgram(const char* filename);
	void emulateCycle();


    //opcodes
    void OP_0nnn();
    void OP_00E0(); //cls
    void OP_00EE();
    void OP_1nnn();
    void OP_2nnn();
    void OP_3xkk();
    void OP_4xkk();
    void OP_5xy0();
    void OP_6xkk();
    void OP_7xkk();
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();
    void OP_9xy0();
    void OP_Annn();
	void OP_Bnnn();
    void OP_Cxkk();
    void OP_Dxyn();
    void OP_Ex9E();
    void OP_ExA1();
    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();

    void OP_NULL();



    using table_func = void(Chip8::*)() ;

    // function ptr table start
    std::array<table_func, 0xF + 1> table;
	std::array<table_func, 0xE + 1> table0;
    std::array<table_func, 0xE + 1> table8;
    std::array<table_func, 0xE + 1> tableE;
    std::array<table_func, 0x65 + 1> tableF;

    void Table0();
    void Table8();
    void TableE();
    void TableF();

    void Cycle();

};


#endif // CHIP_8_