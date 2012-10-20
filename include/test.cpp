#include "CMidi.h"


int main(int argc, char *argv[])
{

string str = argv[1];
string ttl = argv[2];


//***COLOR CHECKING***//
int value = convertToInt(&str[str.length()-1]);

//***BASIC SETUP***//
	Generator gen;
	gen.set_defaultColor(value);
	gen.setTitle(ttl);
	gen.generate_shit();

//***MIDI SETUP***//
	//MIDI mid;
	//mid.setup();
	
	//mid.setChannelNr(value);

	//I took out MIDI class as a reiceiving class
	//I guess he can still be in there but not for performance
	//Maybe it's good to have so if the user wants to save his
	//pattern in a midi file he can do so 
	//The midi class will then take down and register
	//all information about notes, intervals, velocity etc
	//But for now he is completely useless
	//all values or states he could be in belong
	//to the Generator anyway


//***//TESTING	
	while(1)
	{
		gen.getMouse();
		
	}


return 0;
}
