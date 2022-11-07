//
//      Implementation of the Instruction class.
//
#include "stdafx.h"
#include "Instruction.h"

// Parse the Instruction. THIS IS WHERE WE START
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{ 
    m_instruction = a_line;

    // remove any comments
    RemoveComment(a_line);
    
    // parse the line
    bool rv = ParseLine(a_line, m_Label, m_OpCode, m_Operand);

    if (!m_Label.empty() && m_OpCode.empty())
        Errors::RecordError("Quack");

	return ST_MachineLanguage; 
}

void Instruction::RemoveComment(string& a_line)
{
    size_t pos = a_line.find(';');

    if (pos == string::npos)     // indicates there is no position
        return;

    a_line.erase(pos);
}

bool Instruction::ParseLine(const string& a_line, string& a_label, string& a_opcode, string& a_operand)
{
    a_label = a_opcode = a_operand = "";

    istringstream ins(a_line);

    if (a_line.empty())
        return true;

    if (a_line[0] != ' ' && a_line[0] != '\t')
        ins >> a_label;
    
    string temp;
    ins >> a_opcode >> a_operand >> temp;
    
    return temp == "";     

}

// Compute the location of the next instruction.
int Instruction::LocationNextInstruction(int a_loc) 
{ 
	return 100; 
}


