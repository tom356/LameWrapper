#include <iostream>
#include <stdio.h>
#include <fstream>
#include <istream>
#include <vector>
#include <array>

#include <lame/lame.h>
#include "InputParser.h"
#include "LameWrapper.h"
// ./Release/encode -i sounds/ocean.wav -o sounds/ocean.mp3
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
