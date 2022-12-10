//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

/*
NAME

    Assembler Class

SYNOPSIS

    Assembler( int argc, char *argv[] )
        argc    -> number of command line arguements 
        *argv   -> pointer to character array which contains file name

DESCRIPTION

    This class translates a given program on the command line into numeric machine language code,
    fills in a symbol table of labels and non-numeric operands, as well as records errors depending on 
    mistakes found in the program being translated. 
*/

Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{ }  

Assembler::~Assembler() { };

/*
NAME

    PassI - goes through each instruction and fill in symbol table accordingly

SYNOPSIS

    void PassI ( );

DESCRIPTION

    This function will use the FileAccess and Instruction classes to read in
    each line of code from the program file, will aquire the locations of symbols 
    (labels or non-numeric operands), and then enter them into m_SymTab, which is a
    SymTab object. The symbol as well as it's location is found and stored.
*/

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

/*
NAME

    PassII - translates assembly program into machine language

SYNOPSIS

    void PassII ( );

DESCRIPTION

    This function will keep track of the assembler locations of statements, 
    while translating the statements into "Contents", which is a seven digit numerical
    machine language represetnation of an assembly instruction. The first two digits represent
    the operation code, while the following four contain the address. This function will also collect
    errors as it encounters them, and stores them in a vector in the Errors class. 

    This function also prints the original statement that was translated, after translation completes. 
*/

void Assembler::PassII()
{
    // Print header
    cout << "Translation of Program: " << endl << endl;
    cout << "Location\tContents\tOriginal Statement" << endl;

    // Go to beginning of file
    m_facc.rewind();

    int loc = 0;

    // Initalizate error reporting
    Errors::InitErrorReporting();

    // Loop through the program, line by line
    for (; ;)
    {
        // Read the next line from the source file.
        string line;

        if (!m_facc.GetNextLine(line)) {
            // If there are no more lines, we are missing an end statement.
            Errors::RecordError("Assembly instruction END not found");
        }

        // Aquire instructin type
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // Check for errors 
        if (!m_inst.GetLabel().empty() && m_inst.GetOpCode().empty())
            Errors::RecordError("Undefined Label");

        // Print if just a comment line
        if (st == Instruction::ST_Comment)
        {
            cout << "\t\t\t\t" << line << endl;
            continue;  // do not advance in memory if statement was just a comment!
        }

        // Translate assembler instruction
        else if (st == Instruction::ST_AssemblerInstr)
        {
            if (!TranslateAssemInstruction(loc))
            {
                cout << "???\t" << endl;
            }
        }
           
        // Translate machine language instruction
        else if (st == Instruction::ST_MachineLanguage)
        {
            if(!TranslateMachineInstruction(loc))  
                cout << "???\t\t";      // error translating

            // Set location
            loc += 1;
        }

        // Check if end
        else if (st == Instruction::ST_End)
        {
            cout << "\t\t\t\t" << line << endl << endl;
            
            // Ask for user input
            cout << "Press enter to continue...";
            cin.ignore();

            break;
        }
        
        // If not a comment, assembler instruction, or machine instruction, or end, error!
        else
            Errors::RecordError("Invalid instruction");

        // Print the original statement
        cout << line << endl;
    }
    return;
}

/*
NAME

    TranslateAssemInstruction - translates assembly instruction into machine language

SYNOPSIS

    bool TranslateAssemInstruction ( int &a_loc )
        &a_loc	-> previous location, passed by reference

DESCRIPTION

    This function translates an assembly language instruction into machine language,
    due to this being an assembly instruction, an ORG or DS instruction can change the current location,
    so a_loc is passed by referenced and is updated by this function (if not an ORG or DS instruction,
    the function simply iterates through a_loc). This function also prints the translated code and
    current location. Returns false if an error occured translating.
*/

bool Assembler::TranslateAssemInstruction(int &a_loc)
{
    // Get the OpCode
    string cmpOpCode = m_inst.MatchCase(m_inst.GetOpCode());

    // Print current location
    cout << a_loc << "\t\t";

    // If org, set location accordingly 
    if (m_inst.CheckORG(a_loc))
    {
        cout << "\t\t";
        a_loc = m_inst.Instruction::GetNumOperand();
        return true;
    }

    // Check for operand
    if (m_inst.GetStringOperand().empty())
    {
        Errors::RecordError("Missing Operand");
        return false;
    }

    // Assign contents and fill in memory
    int contents;
    if (cmpOpCode == "DC")      // Define constant, just inserts contents at memory location
    {
        // Assign contents, print, and store in memory
        contents = m_inst.GetNumOperand();
        cout << setfill('0') << setw(7) << contents << "\t\t";
        
        // When storing in memory, if unsuccessful record an error 
        if (contents > Emulator::MEMSZ)
        { 
            Errors::RecordError("Constant size too large for computer");
            return false;
        }

        else if (!m_emul.insertMemory(a_loc, contents))
        {
            Errors::RecordError("Overwriting memory!");
            return false;
        }
            
    }
        
    else if (cmpOpCode == "DS")    // Define storage, skips defined number of lines in assembler memory
    {
        a_loc += m_inst.GetNumOperand();
        cout << "\t\t";
        return true;
    }

    // Update location
    a_loc += 1;

    return true;
}

/*
NAME

    TrandlateMachineInstruction - translates machine instruction into machine language

SYNOPSIS

    bool TranslateMachineInstruction ( int a_loc )
        a_loc	-> previous location

DESCRIPTION

    This function translates a machine language instruction into machine language,
    due to this being a machine instruction, the location is passed as a copy and not updated here,
    but in the PassII function. This function also prints the translated code and
    current location. Returns false if an error occured translating.
*/

bool Assembler::TranslateMachineInstruction(const int a_loc)
{
    // Show location of statement
    cout << a_loc << "\t\t";

    // Get numeric OpCode
    int numOpCode = m_inst.GetNumericOpCode(),
        symbolLocation = 0;

    if (m_inst.GetStringOperand().empty() && numOpCode != 13)
        Errors::RecordError("Missing Operand");

    // Find symbol's address, record error if it does not exist and this isn't halt statement
    if (!m_symtab.LookupSymbol(m_inst.GetStringOperand(), symbolLocation) && numOpCode != 13)
    {
        Errors::RecordError("Attempting to access symbol which has not been defined");
        return false;
    }

    else if (symbolLocation == SymbolTable::multiplyDefinedSymbol)
    {
        Errors::RecordError("Accessing a multiply defined symbol");
        return false;
    }

    // Halt statement, no symbol but not an error
    else if (numOpCode == 13)  
        symbolLocation = 0;

    // Assign contents, print, and store in memory
    int contents = numOpCode * 10'000 + symbolLocation;
    cout << setfill('0') << setw(7) << contents << "\t\t";
    
    // When storing in memory, if unsuccessful record an error 
    if (a_loc > Emulator::MEMSZ)
    {
        Errors::RecordError("Out of memory / assigning out of memory bounds");
        return false;
    }

    else if (!m_emul.insertMemory(a_loc, contents))
    {
        Errors::RecordError("Overwriting memory!");
        return false;
    }
        
    // Return true if successful translation
    return true;
}

/*
NAME

    RunProgramInEmulator - calls Emulator class function to run the emulator, unless there are errors

SYNOPSIS

    RunProgramInEmulator (  )

DESCRIPTION

    This function calls the run proram funcion of the emulator class, by running 
    that function on the m_emul object. Before running, checks for errors
*/

void Assembler::RunProgramInEmulator()
{
    // Check for errors
    if (Errors::ErrorsExist())
        Errors::DisplayErrors();
    
    // Print header
    else 
    {
        cout << "Results from emulating program:" << endl << endl;

        m_emul.runProgram();

        cout << endl << "End of emulation" << endl;
    }
    
    return;
}

/*
NAME

    DisplaySymbolTable - calls SymbTab class function to display symbol table

SYNOPSIS

    DisplaySymbolTable (  )

DESCRIPTION

    This function calls the display symbol table funcion of the symbol table class, by running
    that function on the m_symtab object
*/

void Assembler::DisplaySymbolTable()
{
    m_symtab.DisplaySymbolTable();

    // Ask for user input
    cout << "---------------------------------" << endl << endl;
    cout << "Press enter to continue...";
    cin.ignore();
    return;
}