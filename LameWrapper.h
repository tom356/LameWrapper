#ifndef LAME_WRAPPER_H
#define LAME_WRAPPER_H

#include<string>
#include<vector>
#include<fstream>

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
		char id[4];	// "data"
		int32_t size;
		std::vector<char> data;	
		DATA();
	};
	struct WAVE
	{
		RIFF riff;
		FMT fmt;
		DATA data;
		WAVE();
	};

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
	bool loadWave(std::string filePath, WAVE &wf);
	void init_lame_params(lame_t &lame, WAWE &wf);
	void waveToMp3();
}
#endif