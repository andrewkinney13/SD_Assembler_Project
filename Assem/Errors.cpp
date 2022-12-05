//
//      Implementation of the Errors class.
//
#include "stdafx.h"
#include "Errors.h"

/*
NAME

    Errors Class

SYNOPSIS

    No constructor

DESCRIPTION

    Stores error messages when encountered in other class functions
*/

vector<string> Errors::m_ErrorMsgs;

/*
NAME

    InitErrorReporting - Initalizes error reporting class

SYNOPSIS

    void InitErrorReporting( );

DESCRIPTION

     Clears error messages object, which is just a vector of strings.
*/

void Errors::InitErrorReporting()
{
	m_ErrorMsgs.clear();
}

/*
NAME

    RecordError - Records an error

SYNOPSIS

    void RecordError( string a_emsg );
        string a_emsg      -> passed in error message

DESCRIPTION

     Adds a passed in string to the error messages vector member variable
*/

void Errors::RecordError(string a_emsg)
{
	m_ErrorMsgs.push_back(a_emsg);
}

/*
NAME

    DisplayErrors - Displays errors found

SYNOPSIS

    void DisplayErrors( );

DESCRIPTION

     Prints all the recorded errors found while translating the program
*/

void Errors::DisplayErrors()
{
    for (string msg : m_ErrorMsgs)
        cout << msg << endl;
}