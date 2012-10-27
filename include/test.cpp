#include <iostream>
#include <cstdlib>
#include "Cmidi.h"


int main( int argc, char *argv[] )
{
   
    int LOOP = 0; 
    string str = argv[1];
    string ttl = argv[2];

//is BPM specified??
	if(argc > 3)
	{
    		BPM = convertToInt(argv[4]); 
	}
	else
	{
		BPM = 120;
	}
//is loop specified??
     if((argc > 5) && (convertToInt(argv[6]) < width-1))
     { 
	LOOP = convertToInt(argv[6]); 
     }
    else
    {
	//loop can never be bigger than this
	LOOP = width-2;
    }
    //***COLOR CHECKING***//
    int value = convertToInt(&str[str.length()-1]);
    
    //***BASIC SETUP***//
    Cmidi mid;
	mid.set_defaultColor(value);
	mid.getLoop(LOOP); //get loop sequence
	mid.setTitle(ttl); //set title
	mid.generate_shit(); //do it!
    
    

    
    // Call function to select port.
    try {
        if ( mid.chooseMidiPort() == false );
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
    

    
    while(1)
    {
        
                mid.getInput();
                
    }	
    
    
    return 0;
}

