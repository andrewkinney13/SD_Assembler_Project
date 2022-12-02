//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

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
}

// Lookup a symbol in the symbol table.
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