#include<iostream>
#include "InputParser.h"
using namespace LameWrapper;

Params::Params():input(""),output(""){}

void LameWrapper::setParam(Options option, std::string value, Params &p)
{
	switch(option)
	{
		case OPTIONS_INPUT:	p.input = value; break;
		case OPTIONS_OUTPUT: p.output = value; break;
		default: break;
	}
}

Options LameWrapper::getOptionType(const std::string &s)
{
	if(s == "-i") return OPTIONS_INPUT;
	else if(s == "-o") return OPTIONS_OUTPUT;
	else return OPTIONS_INVALID;
}
int LameWrapper::parseInput(int argc, char**argv, Params &params)
{
	std::string param,value;
	Options type;
	if(argc < 5)
	{
		std::cout << " not enough parameters " << std::endl;
		return 3; // not enough parameters
	}
	for(int i=1;i<argc;i+=2)
	{
		param = argv[i];		
		type = getOptionType(param);
		if(type == OPTIONS_INVALID)
		{
			std::cout << " unknown parameter: "<< param << std::endl;
			return ERROR_UNKNOWN_PARAM;
		}				
		if(i+1<argc)
		{
			value = argv[i+1];
			if(value[0]=='-')
			{
				std::cout << "no value for parameter: " << param << std::endl;
				return ERROR_NO_VALUE;
			}
			setParam(type, value, params);
		}
		else
		{
			std::cout << "no value for parameter: " << param << std::endl;
			return ERROR_NO_VALUE;
		}
	}
	return 0;
}