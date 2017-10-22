#include <iostream>
#include <stdio.h>
#include <fstream>
#include <istream>
#include <vector>
#include <array>

#include <lame/lame.h>
#include "InputParser.h"
#include "LameWrapper.h"

int main(int argc, char**argv )
{	
	/*
	std::string file{"./ocean.wav"};	
	lameWrapper::WAVE waveFile;
	lameWrapper::loadWave(file,waveFile);
	lameWrapper::printWaveInfo(waveFile);
	*/
	lameWrapper::Params p;
	if(lameWrapper::parseInput(argc, argv, p) == 0)		
		lameWrapper::waveToMp3(p.input, p.output);
	else
		std::cout << "parsing failed" << std::endl;
	
	return 0;
}
