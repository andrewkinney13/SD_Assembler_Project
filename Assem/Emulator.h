//
//		Emulator class - supports the emulation of VC8000 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class Emulator {

public:

    const static int MEMSZ = 10'000;	// The size of the memory of the VC407.
    Emulator() {

         m_memory.resize(MEMSZ, 0);
    }
    // Records instructions and data into simulated memory.
    bool insertMemory( int a_location, long long a_contents );
    
    // Runs the program recorded in memory.
    bool runProgram( );

private:

    vector<int> m_memory;  	// Memory for the VC407
};

#endif

