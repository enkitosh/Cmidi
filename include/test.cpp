#include "CMidi.h"


int main(int argc, char *argv[])
{

string str = argv[1];
string ttl = argv[2];

//***COLOR CHECKING***//
int value = checkColor(&str[str.length()-1]);

//***BASIC SETUP***//
	Generator gen;
	gen.set_defaultColor(value);
	gen.setTitle(ttl);
	gen.generate_shit();
	

//***//TESTING	
	while(1)
	{
		gen.getMouse();
	}


return 0;
}
