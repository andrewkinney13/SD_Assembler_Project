//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}  
// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler( )
{

}
// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );
        
        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::ST_Comment )  
        {
        	continue;
	    }

        // If this statement contains ORG instruction, set the origin location
        if (m_inst.Instruction::CheckORG( loc ))
        {
            loc = m_inst.Instruction::GetNumOperand();
            continue;
        }

        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {
            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}

// This will translate the code... oh boy!
void Assembler::PassII()
{
    // header
    cout << "Translation of Program: " << endl << endl;
    cout << "Location\tContents\tOriginal Statement" << endl;

    // go to beginning of file
    m_facc.rewind();

    int loc = 0;

    // initalizate error reporting
    Errors::InitErrorReporting();

    // loop through the program 
    for (; ;)
    {
        // Read the next line from the source file.
        string line;

        if (!m_facc.GetNextLine(line)) {
            // If there are no more lines, we are missing an end statement.
            Errors::RecordError("Assembly instruction END not found");
        }

        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        if (st == Instruction::ST_Comment)
        {
            cout << "\t\t\t\t" << line << endl;
            continue;  // do not advance in memory if statement was just a comment!
        }

        else if (st == Instruction::ST_AssemblerInstr)
            TranslateAssemInstruction(loc);
            
        else if (st == Instruction::ST_MachineLanguage)
        {
            TranslateMachineInstruction(loc);

            // Set location
            loc += 1;
        }
            
        // Check if end
        if (m_inst.CheckEND())
        {
            cout << "\t\t\t\t" << line;
            break;
        }

        // Print the original statement
        cout << line << endl;
        
    }

    return;
}

// Displays symbols in symbol table
void Assembler::DisplaySymbolTable() 
{ 
    m_symtab.DisplaySymbolTable();
    return;
}

void Assembler::TranslateAssemInstruction(int &a_loc)
{
    // get the OpCode
    string cmpOpCode = m_inst.MatchCase(m_inst.GetOpCode());

    // print current location
    cout << a_loc << "\t\t";

    // if org, set location accordingly 
    if (m_inst.CheckORG(a_loc))
    {
        cout << "\t\t";
        a_loc = m_inst.Instruction::GetNumOperand();
        return;
    }

    // assign contents and fill in memory
    int contents;
    if (cmpOpCode == "DC")
    {
        contents = m_inst.GetNumOperand();
        cout << contents << "\t\t";
        m_emul.insertMemory(a_loc, contents);
    }
        

    else if (cmpOpCode == "DS")
    {
        a_loc += m_inst.GetNumOperand();
        cout << "\t\t";
        return;
    }

    // Set location
    a_loc += 1;

    return;
}

void Assembler::TranslateMachineInstruction(const int a_loc)
{
    // show location of statement
    cout << a_loc << "\t\t";

    int numOpCode = m_inst.GetNumericOpCode(),
        symbolLocation = 0;

    if (numOpCode == -1)
    {
        Errors::RecordError("Invalid Machine Language OpCode");
        return;
    }

    // find symbol's address
    if (!m_symtab.LookupSymbol(m_inst.GetStringOperand(), symbolLocation) && numOpCode != 13)
    {
        Errors::RecordError("Attempting to access symbol which has not been defined");
        return;
    }

    // halt statement, no symbol but not an error
    else if (numOpCode == 13)  
        symbolLocation = 0;

    // assign contents, print, and store in memory
    int contents = numOpCode * 10'000 + symbolLocation;
    cout << contents << "\t\t";
    m_emul.insertMemory(a_loc, contents);

    return;
}

void Assembler::RunProgramInEmulator()
{

}
