#ifndef LAME_WRAPPER_H
#define LAME_WRAPPER_H

#include<string>
#include<vector>
#include<fstream>
#include<lame/lame.h>

namespace lameWrapper
{
	struct RIFF
	{
		char id[4];			// RIFF
		int32_t size; 		// size of thins
		char format[4]; 	// expecting "WAVE"	
		RIFF();
	};
	struct FMT
	{
		char id[4];	// "fmt "
		int32_t size; 	// size of things
		int16_t formatcode;
		int16_t channels;
		int32_t samplerate;
		int32_t avgbps;
		int32_t blockalign;
		int16_t bps;
		FMT();
	};
	struct DATA
	{
		char id[4];
		int32_t size;
		std::vector<char> samples;	
		DATA();
	};
	struct WAVE
	{
		RIFF riff;
		FMT fmt;
		DATA data;
		WAVE();
	};
	
	enum WAVEFormatCode{WAVE_FORMAT_PCM = 1, WAVE_FORMAT_IEEE_FLOAT = 3, WAVE_FORMAT_ALAW = 6, WAVE_FORMAT_MULAW = 7, WAVE_FORMAT_EXTANSIBLE = 65534};
	
	// all below could be hidden
	bool getRiffID(std::ifstream &file, RIFF &riff);
	bool getRiffSize(std::ifstream &file, RIFF &riff);
	bool getRiffFormat(std::ifstream &file, RIFF &riff);
	bool getFmtID(std::ifstream &file, FMT &fmt);
	bool getFmtSize(std::ifstream &file, FMT &fmt);
	bool getFmtAudioFormat(std::ifstream &file, FMT &fmt);
	bool getFmtNumChannels(std::ifstream &file, FMT &fmt);
	bool getFmtSampleRate(std::ifstream &file, FMT &fmt);
	bool getFmtAvgBps(std::ifstream &file, FMT &fmt);
	bool getFmtBlockAlign(std::ifstream &file, FMT &fmt);
	bool getFmtBps(std::ifstream &file, FMT &fmt);
	bool getDataID(std::ifstream &file, DATA &data);
	bool getDataSize(std::ifstream &file, DATA &data);
	bool getDataSamples(std::ifstream &file, DATA &data);
	int16_t endianSwap(int16_t i);
	int32_t endianSwap(int32_t i);
		
	// this ones are useful
	bool loadWave(const std::string &filePath, WAVE &wf);
	void printWaveInfo(WAVE &wf);
	void init_lame_params(lame_t &lame, WAVE &wf);
	bool encode_8bps(lame_t &lame, WAVE &wave_file, std::vector<unsigned char> &mp3_buffer, std::ofstream &mp3File);
	bool encode_16bps(lame_t &lame, WAVE &wave_file, std::vector<unsigned char> &mp3_buffer, std::ofstream &mp3File);
		
	bool waveToMp3(const std::string &in, const std::string &out);
}
#endif