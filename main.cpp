#include <iostream>

#include "InputParser.h"
#include "LameWrapper.h"

int main(int argc, char**argv )
{	

	lamewrapper::Params p;
	if( lamewrapper::parseInput(argc, argv, p) == 0 )
	{	
		lamewrapper::Encoder e;
		e.encode(p.input, p.output);
	}
	
	return 0;
}
