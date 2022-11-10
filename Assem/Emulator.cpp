//
//      Implementation of the Emulator class.
//
#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"

// Records instructions and data into simulated memory.
bool Emulator::insertMemory(int a_location, int a_contents)
{
	//m_memory[a_location] = a_contents

	return true;
}

// Runs the program recorded in memory.
bool Emulator::runProgram()
{
	//loop through the memory addresses starting at location 100
	//int insr = m_memort[loc];
	//int opcode = instr / 10'000 (this is how the address / offset are formatted)
	//int addr = instr % 10'000 (for remainder)
	//massive switch statement based on the opcode, for all the opcodes
		//for adding, case 1, acc = acc + m_memory[addr]
		//loc++
		//break

	return true;
}