#include "chip8.h"
#include <cstring>

#define pc programCounter
#define opc opcode

constexpr unsigned int start_adress = 0x200;




Chip8::Chip8() : ramdGen(std::chrono::system_clock::now().time_since_epoch().count()){
	// Constructor implementation
	initialize();
	randByte = std::uniform_int_distribution<uint8_t>(0, 255U); // Random byte generator for sound and delay timers

    table.at(0x0) = &Chip8::Table0;
    table.at(0x1) = &Chip8::OP_1nnn;
    table.at(0x2) = &Chip8::OP_2nnn;
    table.at(0x3) = &Chip8::OP_3xkk;
    table.at(0x4) = &Chip8::OP_4xkk;
    table.at(0x5) = &Chip8::OP_5xy0;
    table.at(0x6) = &Chip8::OP_6xkk;
    table.at(0x7) = &Chip8::OP_7xkk;
    table.at(0x8) = &Chip8::Table8;
    table.at(0x9) = &Chip8::OP_9xy0;
    table.at(0xA) = &Chip8::OP_Annn;
    table.at(0xB) = &Chip8::OP_Bnnn;
    table.at(0xC) = &Chip8::OP_Cxkk;
    table.at(0xD) = &Chip8::OP_Dxyn;
    table.at(0xE) = &Chip8::TableE;
    table.at(0xF) = &Chip8::TableF;

    for(size_t i = 0; i < 0xE; i++){
      table0[i] = &Chip8::OP_NULL;
      table8[i] = &Chip8::OP_NULL;
      tableE[i] = &Chip8::OP_NULL;
    }


    table0.at(0x0) = &Chip8::OP_00E0;
    table0.at(0xE) = &Chip8::OP_00EE;

    table8.at(0x0) = &Chip8::OP_8xy0;
    table8.at(0x1) = &Chip8::OP_8xy1;
    table8.at(0x2) = &Chip8::OP_8xy2;
    table8.at(0x3) = &Chip8::OP_8xy3;
    table8.at(0x4) = &Chip8::OP_8xy4;
    table8.at(0x5) = &Chip8::OP_8xy5;
    table8.at(0x6) = &Chip8::OP_8xy6;
    table8.at(0x7) = &Chip8::OP_8xy7;
    table8.at(0xE) = &Chip8::OP_8xyE;

    tableE.at(0x1) = &Chip8::OP_ExA1;
    tableE.at(0xE) = &Chip8::OP_Ex9E;

    for (size_t i = 0; i < 0x65; i++) {
      tableF[i] = &Chip8::OP_NULL;
    }

    tableF.at(0x07) = &Chip8::OP_Fx07;
    tableF.at(0x0A) = &Chip8::OP_Fx0A;
    tableF.at(0x15) = &Chip8::OP_Fx15;
    tableF.at(0x18) = &Chip8::OP_Fx18;
    tableF.at(0x1E) = &Chip8::OP_Fx1E;
    tableF.at(0x29) = &Chip8::OP_Fx29;
    tableF.at(0x33) = &Chip8::OP_Fx33;
    tableF.at(0x55) = &Chip8::OP_Fx55;
    tableF.at(0x65) = &Chip8::OP_Fx65;

}

void Chip8::Table0()
{
	((*this).*(table0[opcode & 0x000Fu]))();
}

void Chip8::Table8()
{
	((*this).*(table8[opcode & 0x000Fu]))();
}

void Chip8::TableE()
{
	((*this).*(tableE[opcode & 0x000Fu]))();
}

void Chip8::TableF()
{
	((*this).*(tableF[opcode & 0x00FFu]))();
}

void Chip8::OP_NULL(){}

Chip8::~Chip8() {
	// Destructor implementation
}

void Chip8::Cycle()
{
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	pc += 2;

	((*this).*(table[(opcode & 0xF000u) >> 12u]))();

	if (delayTimer > 0)
		--delayTimer;

	if (soundTimer > 0)
		--soundTimer;
}

void Chip8::initialize() {

	programCounter = 0x200; // Program starts at 0x200
	for (int i = 0; i < FONTSET_SIZE; ++i) {

		memory[FONTSET_START_ADDRESS + i] = fontset[i]; // 0x50 (fontset start address) Load fontset into memory
	}
}

void Chip8::loadProgram(const char* fname) {
	// open the file in binary mode and point to the end of the file
	std::ifstream file(fname, std::ios::binary | std::ios::ate); // open as binary, and point to the end

	if (file.is_open()) {
		// tellg() returns the current position in the file, which is at the end
		// streampos - returns the position in the file as a type that can be used 

		std::streampos size = file.tellg(); // get the size of the file and alloocate a buffer to hold he conteasts
		char* buffer = new char[size];

		// seekg() sets the pos in file
		// std::ios::beg means to go back to the beginning of the file
		// read() reads the contents of the file into the buffer

		file.seekg(0, std::ios::beg); // go back to the beginning of the file
		file.read(buffer, size); // read the contents of the file into the buffer
		file.close(); // close the file

		// copy the contents of the buffer into the memory of the chip8
		for (long i = 0; i < size; i++)
		{
			memory[start_adress + i] = buffer[i]; 
		}
		delete[] buffer; // free the buffer memory
	}
}

void Chip8::OP_0nnn() //
{
	// This is a no-op instruction, it does nothing
	// It is used to skip the next instruction if the previous instruction was a jump
	// or to skip the next instruction if the previous instruction was a call
	// or to skip the next instruction if the previous instruction was a return
	// or to skip the next instruction if the previous instruction was a clear screen
	// or to skip the next instruction if the previous instruction was a scroll down
}

void Chip8::OP_00E0() // CLS
{
	memset(display, 0, sizeof(display)); // Clear the display by setting all pixels to 0
}

void Chip8::OP_00EE() // RET
{
	if (stackPointer > 0) {
		programCounter = stack[--stackPointer]; // Pop the top address from the stack and set the program counter to it
	}

	// basically sp = certain number, deincrementing number means top layer gets ignored in array, basically a pop of stack, srill there but can't be accessed
}

void Chip8::OP_1nnn(){
	uint16_t address = opcode & 0x0FFFu;
	programCounter = address;
}

void Chip8::OP_2nnn(){
	stack[stackPointer] = pc;
  	++stackPointer;
	pc = opcode & 0x0FFFu;
}

void Chip8::OP_3xkk(){

    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opc & 0x00FFu;

    if (registers[Vx] == byte) {
      pc += 2;
    }
}

void Chip8::OP_4xkk(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t byte = opc & 0x00FFu;

  if (registers[Vx] != byte) {
    pc += 2;
  }
}

void Chip8::OP_5xy0(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u;

  if (registers[Vx] == Vy) {
    pc += 2;
  }
}

void Chip8::OP_6xkk(){
  	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opc & 0x00FFu;

    registers[Vx] = byte;
}

void Chip8::OP_7xkk(){
  	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opc & 0x00FFu;

    registers[Vx] += byte;
}

void Chip8::OP_8xy0(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u;

  registers[Vy] = registers[Vx];
}

void Chip8::OP_8xy1(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u;

  registers[Vx] |= registers[Vy];
}

void Chip8::OP_8xy2(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u;

  registers[Vx] &= registers[Vy];
}

void Chip8::OP_8xy3(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u;

  registers[Vx] ^= registers[Vy];
}

void Chip8::OP_8xy4(){
  	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
 	uint8_t Vy = (opcode & 0x00F0) >> 4u;

  	auto result = registers[Vx] + registers[Vy];
  	if (result > 255)
    	registers[0xF] = 1;
  	else
    	registers[0xF] = 0;

	registers[Vx] = result & 0xFF;
}


void Chip8::OP_8xy5()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] > registers[Vy])
		registers[0xF] = 1;
	else
		registers[0xF] = 0;


	registers[Vx] -= registers[Vy];
}

void Chip8::OP_8xy6(){
  	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[Vx] & 0x1u);

	registers[Vx] >>= 1;
}

void Chip8::OP_8xy7(){
  	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vy] > registers[Vx])
      registers[Vy] = 1;
    else
      registers[Vy] = 0;

    registers[Vx] -= registers[Vy];

}

void Chip8::OP_8xyE(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

	registers[Vx] <<= 1;
}

void Chip8::OP_9xy0(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u;

  if (registers[Vx] != registers[Vy])
    pc += 2;
}

void Chip8::OP_Annn(){
  indexRegister = opcode & 0x0FFFu;
}

void Chip8::OP_Bnnn(){
  programCounter = (opcode & 0x0FFFu) + registers[0];
}

void Chip8::OP_Cxkk(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t byte = opc & 0x00FFu;

  registers[Vx] = byte & randByte(ramdGen);
}

void Chip8::OP_Dxyn()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;


	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[indexRegister + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &display[(yPos + row) * VIDEO_WIDTH + (xPos + col)];


			if (spritePixel)
			{

				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}


				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::OP_Ex9E(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  if(keypad[registers[Vx]])
    pc += 2;
}

void Chip8::OP_ExA1(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  if(!keypad[registers[Vx]])
    pc += 2;
}

void Chip8::OP_Fx07(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  registers[Vx] = delayTimer;
}

void Chip8::OP_Fx0A()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keypad[0])
		registers[Vx] = 0;

	else if (keypad[1])
		registers[Vx] = 1;

	else if (keypad[2])
		registers[Vx] = 2;

	else if (keypad[3])
		registers[Vx] = 3;

	else if (keypad[4])
		registers[Vx] = 4;

	else if (keypad[5])
		registers[Vx] = 5;

	else if (keypad[6])
		registers[Vx] = 6;

	else if (keypad[7])
		registers[Vx] = 7;

	else if (keypad[8])
		registers[Vx] = 8;

	else if (keypad[9])
		registers[Vx] = 9;

	else if (keypad[10])
		registers[Vx] = 10;

	else if (keypad[11])
		registers[Vx] = 11;

	else if (keypad[12])
		registers[Vx] = 12;

	else if (keypad[13])
		registers[Vx] = 13;

	else if (keypad[14])
		registers[Vx] = 14;

	else if (keypad[15])
		registers[Vx] = 15;

	else
		pc -= 2;
}

void Chip8::OP_Fx15(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  delayTimer = registers[Vx];
}

void Chip8::OP_Fx18(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  soundTimer = registers[Vx];
}

void Chip8::OP_Fx1E(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  indexRegister += registers[Vx];
}

void Chip8::OP_Fx29(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  indexRegister = FONTSET_START_ADDRESS + (5 * registers[Vx]);
}

void Chip8::OP_Fx33(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = registers[Vx];

	memory[indexRegister + 2] = value % 10;
	value /= 10;

	memory[indexRegister + 1] = value % 10;
	value /= 10;


	memory[indexRegister] = value % 10;
}

void Chip8::OP_Fx55(){

  uint8_t Vx = (opcode & 0x0F00u) >> 8u;

  for (size_t i = 0; i <= Vx; ++i){
    memory[indexRegister + i] = registers[i];
  }
}

void Chip8::OP_Fx65(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;

  for (size_t i = 0; i <= Vx; ++i){
    registers[i] = memory[indexRegister + i];
  }
}