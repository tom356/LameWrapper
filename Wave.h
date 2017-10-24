#ifndef WAVE_H
#define WAVE_H

#include<vector>
#include<fstream>

namespace lamewrapper
{
	enum WAVEFormatCode
	{ 
		WAVE_FORMAT_PCM = 1, 
		WAVE_FORMAT_IEEE_FLOAT = 3, 
		WAVE_FORMAT_ALAW = 6, 
		WAVE_FORMAT_MULAW = 7, 
		WAVE_FORMAT_EXTANSIBLE = 65534
	};
	struct RIFF
	{
		char id[4];			// RIFF
		int32_t size; 		// size of thins
		char format[4]; 	// expecting "WAVE"	
		RIFF();
	};
	struct FMT
	{
		char id[4];				// "fmt "
		int32_t size; 			// size of things
		int16_t format_code;	// 
		int16_t channels;		// number of channels
		int32_t samplerate;		// samples per second
		int32_t avg_bps;		// avarage bits per sample
		int16_t block_align;	// bytes per sample. ie 2channels * 16bits_per_sample = 4bytes
		int16_t bps;			// bits per sample
		int16_t ext_size; 		// for non pcm-data
		int16_t valid_bps; 		// for extensible format
		int32_t channel_mask; 	// for extensible format
		char sub_format[16]; 	// for extensible format
		FMT();
	};
	struct FACT
	{
		char id[4];
		int32_t size;
		int32_t sample_length;		
		FACT();
	};
	struct DATA
	{
		char id[4];
		int32_t size;
		std::vector<char> samples;
		bool padbyte;		
		DATA();
	};
	struct WAVE
	{
		RIFF riff;
		FMT fmt;
		FACT fact;
		DATA data;		
		WAVE();
	};	
	class Wave
	{
		std::ifstream mFile;
		WAVE mWave;
		
		bool getRiffID();
		bool getRiffSize();
		bool getRiffFormat();
		
		bool getFmtID();
		bool getFmtSize();
		bool getFmtAudioFormat();
		bool getFmtNumChannels();
		bool getFmtSampleRate();
		bool getFmtAvgBps();
		bool getFmtBlockAlign();
		bool getFmtBps();
		bool getFmtExtSize(); 		// for non pcm-data, for extensible format
		bool getFmtValidBPS(); 		// for extensible format
		bool getFmtChannelMask(); 	// for extensible format
		bool getFmtSubFormat();		// for extensible format
				
		bool getFactID();			// for non pcm-data, for extensible format
		bool getFactSize();			// for non pcm-data, for extensible format
		bool getFactSampleLength();	// for non pcm-data, for extensible format
		
		bool getDataID();
		bool getDataSize();
		bool getDataSamples();
		bool getPadbyte();
				
		bool checkStreamState(const char* errMessage);
		
		std::string getFormat(WAVEFormatCode format);
		
	public:
		
		bool load(const char* wavePath);
		void printInfo();
		// resizes vector and copies samples
		void copySamples(std::vector<short int> &samples);
		const WAVE &getWave();
		const RIFF &getRiff();
		const FMT &getFmt();
		const FACT &getFact();
		const DATA &getData();
		
		Wave();
		Wave(const char* wavePath);
	};
}
#endif