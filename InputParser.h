#ifndef _INPUT_PARSER_H
#define _INPUT_PARSER_H

#include<string>

namespace lameWrapper
{
	enum Options{OPTIONS_INPUT,OPTIONS_OUTPUT,OPTIONS_INVALID};
	enum ParseErrors{ ERROR_UNKNOWN_PARAM=1, ERROR_NO_VALUE};	
	struct Params
	{
		std::string input;
		std::string output;
		Params();
	};
	int parseInput(int argc, char**argv, Params &params);
	void setParam(Options option, std::string value, Params &p);
	Options getOptionType(const std::string &s);
	int parseInput(int argc, char**argv, Params &params);		
}
#endif