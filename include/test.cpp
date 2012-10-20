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
	MIDI mid;
	mid.setup();
	
	mid.setChannelNr(value);

//***SIGNAL HANDLING***/
	Signal sig;

//***//TESTING	
	while(1)
	{
		gen.getMouse();
		
	//LISTEN TO STATES
		sig.listenGen(gen.sendSignal());
		sig.listenMid(mid.sendSignal());
	}


return 0;
}
