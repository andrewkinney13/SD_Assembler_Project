//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    Symbol Table class

SYNOPSIS
    Constructor is empty, nothing passed in

DESCRIPTION

    This class keeps track of all the gathered symbols in the program
    being translated. Symbols are defined during PassI, and later used during PassII when 
    recording their addresses. 
*/

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/

void SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {
        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*
NAME

    DisplaySymbolTable - displays the symbol table.

SYNOPSIS

    void DisplaySymbolTable (  );

DESCRIPTION

    This function will print every label stored in the symbol table, in alphabetical 
    order.
*/

void SymbolTable::DisplaySymbolTable()
{
    // print header
    cout << "Symbol Table: " << endl << endl
        << "Symbol#\tSymbol\tLocation\t" << endl;
    
    // accumulator value
    int count = 0;

    // loop through map and print symbol with location
    for (auto& a : m_symbolTable)
    {
        cout << count << "\t" << a.first << "\t" << a.second << endl;
        count++;
    }
    return;
}

/*
NAME

    LookupSymbol - updates symbol and location that are passed in

SYNOPSIS

    bool LookupSymbol( const string& a_symbol, int& a_loc );
         const string& a_symbol      -> symbol to be initalized 
         int& a_loc                  -> location of symbol 

DESCRIPTION

    This function is passed a variable for the symbol and location, 
    they are updated once found in the map of symbols and true is returned.
    False is returned if the symbol is not found.
*/

bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
{
    for (auto& a : m_symbolTable)
    {
        if (a_symbol == a.first)
        {
            a_loc = a.second;
            return true;
        }
    }
    return false;
}