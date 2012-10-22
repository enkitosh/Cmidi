#include <iostream>
#include <cstdlib>
#include "Cmidi.h"


int main( int argc, char *argv[] )
{
   
    int LOOP = 0; 
    string str = argv[1];
    string ttl = argv[2];
    BPM = convertToInt(argv[4]); 

     if(convertToInt(argv[6]) < width-1)
     { 
	LOOP = convertToInt(argv[6]); 
     }
    else
    {
	//loop can never be bigger than this
	LOOP = width-1;
    }
    //***COLOR CHECKING***//
    int value = convertToInt(&str[str.length()-1]);
    
    //***BASIC SETUP***//
    Generator gen;
	gen.set_defaultColor(value);
	gen.getLoop(LOOP); //get loop sequence
	gen.setTitle(ttl);
	gen.generate_shit();
    
    

    
    // Call function to select port.
    try {
        if ( gen.chooseMidiPort() == false );
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
    

    
    while(1)
    {
        
                gen.getMouse();
                
    }	
    
    
    return 0;
}

