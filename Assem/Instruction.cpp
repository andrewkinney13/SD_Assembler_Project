//
//      Implementation of the Instruction class.
//
#include "stdafx.h"
#include "Instruction.h"
#include <cstring>

/*
NAME

    ParseInstruction - returns instruction type based on contents of line.

SYNOPSIS

    InstructionType ParseInstruction( string& a_line );
        a_line      ->  line that this function will determine the type of

DESCRIPTION

    This function creates an instruction out of the passed in line, removes comments from it,
    determines if it has too many instructions, initalizes the label, opcode, and operand 
    data members, and based on that info returns the type of instruction that line was. 
*/

Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{ 
    m_instruction = a_line;

    // remove any comments
    RemoveComment(a_line);
    
    // parse the line
    bool empty = ParseLine(a_line, m_Label, m_OpCode, m_Operand);    
    if (!empty)    // too many arguements, just skip the line and report error in PassII()
        return ST_Comment;

    
    
    // return the instruction type
    return GetType();
}

/*
NAME

    RemoveComment - removes anything after a semicolon on a line

SYNOPSIS

    void RemoveComment( string& a_line );
        a_line      ->  line whose comments are being removed

DESCRIPTION

    This function removes any text after encountering the char ';',
    which represents a comment in programs.
*/

void Instruction::RemoveComment(string& a_line)
{
    size_t pos = a_line.find(';');

    if (pos == string::npos)     // indicates there is no position
        return;

    a_line.erase(pos);
}

/*
NAME

    ParseLine - obtains data member values for current instruction, returns 
                false if too many instructions in line

SYNOPSIS

    bool ParseLine( const string& a_line, string& a_label, string& a_opcode, string& a_operand );
        a_line      ->  full line from program
        a_label     ->  label to be initalized
        a_opcode    ->  opcode to be initalized
        a_operand   ->  operand to be initalized

DESCRIPTION

    This function will return false if there are too many instructions on a given line.
    If there are a proper amouin,t. it will fill in the values for the passed by reference data 
    members depending on their location on the line.
*/

bool Instruction::ParseLine(const string& a_line, string& a_label, string& a_opcode, string& a_operand)
{
    // clear old contents
    a_label = a_opcode = a_operand = "";

    istringstream ins(a_line);

    if (a_line.empty())    // either an empty line or comment that got removed
        return true;

    if (a_line[0] != ' ' && a_line[0] != '\t')    // label is always first column
        ins >> a_label;
    
    // fill in data members
    string temp;
    ins >> a_opcode >> a_operand >> temp;
    
    return temp == "";     // returns false if not empty
}

/*
NAME

    LocationNextInstruction - creates the location of the next instruction

SYNOPSIS

    int LocationNextInstruction( int a_loc);
        a_loc	-> previous location

DESCRIPTION

    This function, using the passed in previous location, will calculate the next
    location for the next instructin. Usually a_loc + 1, but sometimes
    more, if the current OpCode defines storage. 
*/

// Compute the location of the next instruction.
int Instruction::LocationNextInstruction(int a_loc) 
{ 
    // if defining storage, skip that much memory 
    if (MatchCase(m_OpCode) == "DS")
    {
        return a_loc + stoi(m_Operand);
    }
   
	return a_loc + 1; 
}

/*
NAME

    MatchCase - creates an all uppercase copy string of orginal passed-in string 

SYNOPSIS

    string MatchCase( const string& a_opcode);
        a_opcode	-> Operation Code to make uppercase

DESCRIPTION

    This function returns a copy of opcode strings to make comparisions 
    without altering the original user code, the returned string is in 
    all upper case letters
*/

string Instruction::MatchCase(const string& a_opcode)
{
    char charOpCode[6] = "";
    for (int i = 0; i < a_opcode.length(); i++)
    {
        charOpCode[i] = toupper(a_opcode[i]);
    }
    string newOpCode = charOpCode;
    return newOpCode;
}

/*
NAME

    GetType - finds the type of instruction based on the contents of the instruction

SYNOPSIS

    InstructionType GetType ( );

DESCRIPTION

    This function returns the type of instruction for the current line being worked on,
    returns comments if there's a suspected error within the line.
*/

Instruction::InstructionType Instruction::GetType()
{
    // create case insensitive copy of m_OpCode for comparison, all uppercase
    string cmp_OpCode = MatchCase(m_OpCode);

    // check if comment
    if (!m_Label.empty() && m_OpCode.empty() ||    // label but no opcode, also an error to be reported in PassII()
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

int Instruction::GetNumericOpCode()
{
    // create comparison OpCode
    string cmp_OpCode = MatchCase(m_OpCode);

    // assign opCodeNum
    if (cmp_OpCode == "ADD")
        return 1;
    else if (cmp_OpCode == "SUBTRACT")
        return 2;
    else if (cmp_OpCode == "MULTIPLY")
        return 3;
    else if (cmp_OpCode == "DIVIDE")
        return 4;
    else if (cmp_OpCode == "LOAD")
        return 5;
    else if (cmp_OpCode == "STORE")
        return 6;
    else if (cmp_OpCode == "READ")
        return 7;
    else if (cmp_OpCode == "WRITE")
        return 8;
    else if (cmp_OpCode == "BRANCH")
        return 9;
    else if (cmp_OpCode == "BM")
        return 10;
    else if (cmp_OpCode == "BZ")
        return 11;
    else if (cmp_OpCode == "BP")
        return 12;
    else if (cmp_OpCode == "HALT")
        return 13;
   
    // error
    return -1;
}


