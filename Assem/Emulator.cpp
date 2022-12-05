//
//      Implementation of the Emulator class.
//
#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"

/*
NAME

	Emulator class

SYNOPSIS
	Constructor is empty, nothing passed in

DESCRIPTION

	This class runs the translated assembly program based off of inserted
	memory addresses fed in during the translation. 
*/

/*
NAME

	insertMemory - inserts a location and contents into the memory array

SYNOPSIS

	insertMemory ( int a_location, int a_contents )
		int a_location		-> memory address 
		int a_contents		-> contents at that address

DESCRIPTION

	This function stores contents (machine language numerical codes) into an array
	m_memory, which will be used later for the full emulation of the program.
*/

bool Emulator::insertMemory(int a_location, int a_contents)
{
	m_memory[a_location] = a_contents;

	return true;
}

/*
NAME

	runProgram - runs the translated program

SYNOPSIS

	runProgram ( )

DESCRIPTION

	This function will decode the contents at each memory index, obtaining the numerical
	opcode and it's assembly address. A giant switch statememnt then performs an action
	depending on the opcode obtained. This function uses an accumulator for mathematical operations. 
*/

bool Emulator::runProgram()
{
	int currentAddress,
		opcode,
		input = 0;
	
	m_acc = 0;

	// step through the memory of the assembler
	for (int index = 0; index < MEMSZ; index++)
	{
		if (m_memory[index] != 0)
		{
			// decode instruction
			opcode = m_memory[index] / 10'000;
			currentAddress = m_memory[index] % 10'000;
			
			//big switch statement based on the contents
			switch (opcode)
			{

			// add
			case 1:
				m_acc += m_memory[currentAddress];
				break;

			// subtract
			case 2:
				m_acc -= m_memory[currentAddress];
				break;

			// multiply
			case 3:
				m_acc *= m_memory[currentAddress];
				break;

			// divide
			case 4:
				m_acc /= m_memory[currentAddress];
				break;

			// load
			case 5:
				m_acc = m_memory[currentAddress];
				break;

			// store
			case 6:
				m_memory[currentAddress] = m_acc;
				break;

			// read
			case 7:
				cout << "? ";
				cin >> input;
				m_memory[currentAddress] = input % 10'000;
				break;

			// write 
			case 8:
				cout << m_memory[currentAddress] << endl;
				break;

			// branch
			case 9:
				index = currentAddress - 1;
				break;

			// branch minus
			case 10:
				if (m_acc < 0)
					index = currentAddress - 1;
				break;

			// branch zero
			case 11:
				if (m_acc == 0)
					index = currentAddress - 1;
				break;

			// branch positive
			case 12:
				if (m_acc > 0)
					index = currentAddress - 1;
				break;

			// halt
			case 13:
				return true;
			}
		}
	}
	return false;
}