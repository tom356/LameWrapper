#ifndef LAME_WRAPPER_H
#define LAME_WRAPPER_H

#include<string>
#include<vector>
#include<fstream>
#include<lame/lame.h>
#include"Wave.h"

namespace lamewrapper
{
	using Mp3Buffer = std::vector<unsigned char>;
	using ShortBuffer = std::vector<short int>;
	
	class Mp3Writer
	{
	
	private:
	
		std::ofstream mFile;
		
	public:
	
		Mp3Writer();
		Mp3Writer(const char *filePath);
		~Mp3Writer();
		
		bool open(const char *filePath);
		void close();
		void writeBuffer(const char *buffer, int byteCount);
		void writeBuffer(Mp3Buffer &buffer, int byteCount);
	};
	
	class Encoder
	{
	private:
		
		lame_t mLame;
		Wave mWave;
		Mp3Writer mMp3Writer;
		Mp3Buffer mMp3Buffer;
		
		void initLameParams();		
		bool checkForEncodingErrors(int write);
		bool initOutputBuffer();
		bool encode_8bps();
		bool encode_16bps();
		bool encode_24bps();
		bool encode_32bps();
		bool encode_64bps();
		bool flush();
		bool failedAndCleanup();
		void cleanup();
		
	public:
		Encoder();
		~Encoder();
		bool encode(const std::string &in, const std::string &out);
	};

}
#endif