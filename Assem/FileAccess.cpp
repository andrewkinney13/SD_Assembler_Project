//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/*
NAME

    File Access Class

SYNOPSIS

    FileAccess( int argc, char *argcv[] );
        argc    -> number of command line arguements 
        *argv   -> pointer to character array which contains file name

DESCRIPTION

    This function accesses lines of the text program file being translated,
    it obtains the file name from the command line, and will throw an error if 
    no file name is included, or the file itself does not exist in the proper directory.
*/

FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}

/*
NAME

    File Access Destructor

SYNOPSIS

    ~FileAccess( );
       

DESCRIPTION

    Closes the program text file when called.
*/

FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}

/*
NAME

    GetNextLine - updates a_line variable

SYNOPSIS

    GetNextLine( string &a_line )
        &a_line     -> line being updated

DESCRIPTION

    Updates a_line, which is passed in by reference, with the next line of the program,
    and returns true. If no next line exists, returns false.
*/

bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}

/*
NAME

    Rewind - goes to beginning of file

SYNOPSIS

    rewind ( )

DESCRIPTION

    Resets file flags and goes back to the beginning of the file.
*/

void FileAccess::rewind( )
{
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
    
