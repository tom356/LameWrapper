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
	{
		std::vector<char> charVec{0,100,-100,-1};
		std::vector<short int> sintVec;
		
		sintVec.resize(charVec.size());
		for(unsigned int i=0;i<charVec.size();i++)
			sintVec[i] = static_cast<short int>(static_cast<unsigned char>(charVec[i]));

		for(auto &v:charVec)
			std::cout << "(" << v << ")" << ",";
		std::cout << std::endl;
		for(auto &v:sintVec)
			std::cout << v << " ";
		std::cout << std::endl;
	
	}
	
	lamewrapper::Params p;
	if( lamewrapper::parseInput(argc, argv, p) == 0 )
	{	
		lamewrapper::Encoder e;
		e.encode(p.input, p.output);
	}
	
	return 0;
}
