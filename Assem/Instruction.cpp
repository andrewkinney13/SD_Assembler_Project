//
//      Implementation of the Instruction class.
//
#include "stdafx.h"
#include "Instruction.h"
#include <cstring>

// Parse the Instruction. THIS IS WHERE WE START
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{ 
    m_instruction = a_line;

    // remove any comments
    RemoveComment(a_line);
    
    // parse the line
    bool empty = ParseLine(a_line, m_Label, m_OpCode, m_Operand);    

    // create case insensitive copy of m_OpCode for comparison, all uppercase
    char char_OpCode[6] = "";
    for (int i = 0; i < m_OpCode.length(); i++)
    {
        char_OpCode[i] = toupper(m_OpCode[i]);
    }
    string cmp_OpCode = char_OpCode;

    // check if comment
    if (!empty ||    // too many arguements, just skip the line and report error in PassII()
        !m_Label.empty() && m_OpCode.empty() ||    // label but no opcode, also an error to be reported in PassII()
        m_Label.empty() && m_OpCode.empty() && m_Operand.empty())     // line was blank / comment, no contents 
        return ST_Comment;

    // check if contains assembly instructions
    else if (cmp_OpCode == "DC" ||
        cmp_OpCode == "DS" ||
        cmp_OpCode == "ORG")
        return  ST_AssemblerInstr;

    // check if contains symbolic machine language operation code
    else if (cmp_OpCode == "ADD" ||
        cmp_OpCode == "SUB" ||
        cmp_OpCode == "MULT" ||
        cmp_OpCode == "DIV" ||
        cmp_OpCode == "LOAD" ||
        cmp_OpCode == "STORE" ||
        cmp_OpCode == "READ" ||
        cmp_OpCode == "WRITE" ||
        cmp_OpCode == "B" ||
        cmp_OpCode == "BM" ||
        cmp_OpCode == "BZ" ||
        cmp_OpCode == "BP" ||
        cmp_OpCode == "HALT")
        return ST_MachineLanguage;

    // check if operation code is end
    else if (cmp_OpCode == "END")
        return ST_End;

    // some type of error, will be dealt with in PassII()
    else
        return ST_Comment;
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
    // clear old contents
    a_label = a_opcode = a_operand = "";

    istringstream ins(a_line);

    if (a_line.empty())    // either an empty line or comment that got removed
        return true;

    if (a_line[0] != ' ' && a_line[0] != '\t')    // label is always first column
        ins >> a_label;
    
    string temp;
    ins >> a_opcode >> a_operand >> temp;
    
    return temp == "";     // returns false if not empty
}

// Compute the location of the next instruction.
int Instruction::LocationNextInstruction(int a_loc) 
{ 
	return 100; 
}


