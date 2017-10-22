#include <iostream>
#include <stdio.h>
#include <fstream>
#include <istream>
#include <vector>
#include <array>
//#include <stdint>

#include <lame/lame.h>
#include "InputParser.h"
#include "LameWrapper.h"

int convert(const std::string &input, const std::string &output)
{
    int read, write;

    FILE *pcm = fopen("./ocean.wav", "rb");
    FILE *mp3 = fopen("./ocean.mp3", "wb");
	
	if(!pcm) std::cout << "cannot open file ocean.wav" << std::endl;
	if(!mp3) std::cout << "cannot open file ocean.wav" << std::endl;
    
	const int PCM_SIZE = 8192; // how much bytes to read at one time
    const int MP3_SIZE = 8192; // how much bytes to read at one time

    short int pcm_buffer[PCM_SIZE*2];
    unsigned char mp3_buffer[MP3_SIZE];

    lame_t lame = lame_init();
    lame_set_in_samplerate(lame, 44100);
    lame_set_VBR(lame, vbr_default);
    lame_init_params(lame);

	std::cout << "lame initialized" << std::endl;
	
    do {
        read = fread(pcm_buffer, 2*sizeof(short int), PCM_SIZE, pcm);
		std::cout << read << " bytes read, also 2*sizeof(short int) = " << 2*sizeof(short int) << std::endl;
        if (read == 0)
            write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);	
        else
            write = lame_encode_buffer_interleaved(lame, pcm_buffer, read, mp3_buffer, MP3_SIZE);
		std::cout << write << " write chunk size" << std::endl;
		std::cout << "writing" << std::endl;
        fwrite(mp3_buffer, write, 1, mp3);
		std::cout << "wrote" << std::endl;
    } while (read != 0);

    lame_close(lame);
    fclose(mp3);
    fclose(pcm);

    return 0;
}

enum WAVEFormatCode{WAVE_FORMAT_PCM = 1, WAVE_FORMAT_IEEE_FLOAT = 3, WAVE_FORMAT_ALAW = 6, WAVE_FORMAT_MULAW = 7, WAVE_FORMAT_EXTANSIBLE = 65534};
/*
struct FMTChunk
{
	char ckID[4];
	int32_t cksize;
	char WAVEID[4]; // "WAVE"
	
	int16_t formatcode;
	int16_t channels;
	int32_t samplerate;
	int32_t avgbps;
	int32_t blockalign;
	int16_t bps;
	int16_t cbsize;
	
};*/

struct WaveFile
{
	char ckID[4];		// RIFF
	int32_t cksize; 	// size of thins
		char WAVEID[4]; 	// "WAVE"	
		char subckID[4];	// "fmt "
		int32_t subcksize; 	// size of things
			int16_t formatcode;
			int16_t channels;
			int32_t samplerate;
			int32_t avgbps;
			int32_t blockalign;
			int16_t bps;
		char subck2ID[4];	// "data"
		int32_t sub2cksize;
		std::vector<char> data;
		
	WaveFile():samplerate(0),bps(0),channels(0){}
};

int16_t reverseEndian(int16_t i)
{
	return (i&0xff)<<8 || (i&0xff00)>>8;
}
int32_t reverseEndian(int32_t i)
{
	return (i&0xff)<<24 || (i&0xff00)<<8 || (i&0xff0000)>>8 || (i&0xff000000)>>24;
		//return (i&0xff)<<24 || (i&0xff00)<<8 || (i&0xff0000)>>8 || (i&0xff000000)>>24;
	//ff000000
	//00ff0000
	//0000ff00
	//000000ff
}

void extract_and_print(std::ifstream &wave, WaveFile &wf)
{
	const int PCM_SIZE = 8192; // how much bytes to read at one time
	short int pcm_buffer[PCM_SIZE*2];
	
	// get ckID
	char ckID[5];
	wave.read((char*)ckID,4);
	std::cout << "ckID: " << ckID[0]<< ckID[1]<< ckID[2]<< ckID[3] << std::endl;	
	for(int i=0;i<4;i++)
		wf.ckID[i] = ckID[i];
	
	// get cksize
	int32_t cksize[4]{0};
	wave.read((char*)cksize, 4); // chunk size
	std::cout << "cksize: " << cksize[0] << std::endl;
	wf.cksize = cksize[0];

	// get WAVEID
	char WAVEID[5];
	wave.read((char*)WAVEID,4);
	std::cout << "ckID: " << WAVEID[0]<< WAVEID[1]<< WAVEID[2]<< WAVEID[3] << std::endl;	
	for(int i=0;i<4;i++)
		wf.WAVEID[i] = WAVEID[i];
	
	// get subckID
	char subckID[5];
	wave.read((char*)subckID,4);
	std::cout << "subckID: " << subckID[0]<< subckID[1]<< subckID[2]<< subckID[3] << std::endl;	
	for(int i=0;i<4;i++)
		wf.subckID[i] = subckID[i];	
		
	// get subcksize
	int32_t subcksize[4]{0};
	wave.read((char*)subcksize, 4); // sample rate	
	std::cout << "subcksize: " << subcksize[0] << std::endl;
	wf.subcksize = subcksize[0];
	
	// formatcode
	int16_t formatcode[2]{0};
	wave.read((char*)formatcode,2);
	std::cout << "formatcode: " << formatcode[0] << std::endl;	
	wf.formatcode = formatcode[0];
	
	// get channels
	int16_t channels[2]{0};
	wave.read((char*)channels,2);
	std::cout << "channels: " << channels[0] << std::endl;	
	wf.channels = channels[0];
		
	// get samplerate
	int32_t samplerate[4]{0};
	wave.read((char*)samplerate, 4);
	std::cout << "samplerate: " << samplerate[0] << std::endl;
	wf.samplerate = samplerate[0];

	// get avarage bps
	int32_t avgbps[4]{0};
	wave.read((char*)avgbps, 4);
	std::cout << "avg bps: " << avgbps[0] << std::endl;
	wf.avgbps = avgbps[0];	
	
	// get blockalign
	int16_t blockalign[2]{0};
	wave.read((char*)blockalign, 2); // sample rate	
	std::cout << "blockalign: " << blockalign[0] << std::endl;
	wf.blockalign = blockalign[0];	
	
	//get bps
	int16_t bps[2]{0};
	wave.read((char*)bps, 2); // sample rate	
	std::cout << "bps: " << bps[0] << std::endl;
	wf.bps = bps[0];
	
	// get subck2ID
	char subck2ID[5];
	wave.read((char*)subck2ID,4);
	std::cout << "subck2ID: " << subck2ID[0]<< subck2ID[1]<< subck2ID[2]<< subck2ID[3] << std::endl;	
	for(int i=0;i<4;i++)
		wf.subck2ID[i] = subck2ID[i];	
	
	//get sub2cksize
	int32_t sub2cksize[4]{0};
	wave.read((char*)sub2cksize, 4); // sample rate	
	std::cout << "sub2cksize: " << sub2cksize[0] << std::endl;	
	wf.sub2cksize = sub2cksize[0];
	
	//load data
	wf.data.resize( wf.sub2cksize );
	std::cout << "data container size: " << wf.data.size() << std::endl;
	wave.read(&wf.data[0], wf.sub2cksize); // sample rate	
	std::cout << "data loaded: " << wf.sub2cksize << std::endl;
	/*
	// skip
	wave.read((char*)pcm_buffer,2);  // skip some params
	std::cout << "skipped" << std::endl;
	*/
	//decode on fly ?
}
void init_lame_params(lame_t &lame, WaveFile &wf)
{
	
	lame_set_in_samplerate(lame, wf.samplerate);
	lame_set_num_samples(lame, wf.data.size());
    lame_set_brate(lame, wf.samplerate * wf.bps * wf.channels );
	lame_set_findReplayGain(lame, 1);
	//lame_set_brate(lame, wf.avgbps);
    lame_set_num_channels(lame, wf.channels );	
	if(wf.channels == 1) lame_set_mode(lame, MONO );	
	lame_set_VBR(lame, vbr_default);
	lame_set_quality(lame, 0);
	lame_init_params(lame);	
}
void convert_STL()
{
	std::string in{"./ocean.wav"}, out{"./ocean.mp3"};
	std::ifstream wave(in.c_str(), std::ios_base::binary | std::ios_base::in);
	std::ofstream mp3(out.c_str(), std::ios_base::binary | std::ios_base::out);
	
	if(!wave.is_open() || wave.fail())
	{
		std::cout << "cannot open file: " << in << std::endl;
		return;
	}
	if(!mp3.is_open() || mp3.fail())
	{
		std::cout << "cannot open file: " << out << std::endl;
		return;
	}	
	
	WaveFile wf;
	extract_and_print(wave, wf);	
	lame_t lame = lame_init();
	init_lame_params(lame, wf);
		
	const int PCM_SIZE = 8192; // how much bytes to read at one time
    const int MP3_SIZE = 8192; // how much bytes to read at one time

    short int pcm_buffer[PCM_SIZE*2];
    unsigned char mp3_buffer[MP3_SIZE];	
	
	std::cout << "All Set..." << out << std::endl;
	int write{0};
	int offset{0};
	int maxread{0};
	while(offset < wf.data.size())
	{
		maxread = (offset + PCM_SIZE < wf.data.size()?PCM_SIZE:wf.data.size()-offset);
		std::cout << "Reading: " << maxread <<" at offset:" << offset << std::endl;		
		//write = lame_encode_buffer(lame, reinterpret_cast<short int*>( &wf.data[offset] ), NULL, maxread, mp3_buffer, MP3_SIZE);				
		write = lame_encode_buffer_interleaved(lame, reinterpret_cast<short int*>( &wf.data[offset] ), maxread, mp3_buffer, MP3_SIZE);				
		std::cout << "writing: " << write << std::endl;
		mp3.write(reinterpret_cast<char *>(mp3_buffer), write); // a bit more o problems		
		offset+=maxread;
	}
	write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
	std::cout << "flushing, writing: " << write << std::endl;
	mp3.write(reinterpret_cast<char *>(mp3_buffer), write);
	std::cout << "Done" << out << std::endl; 
	/*
	int write{0};
	while(!wave.eof())
	{
		std::cout << "Loop start" << out << std::endl;		
		wave.read((char*)pcm_buffer,PCM_SIZE); // is this bad or not ?
		std::cout << "reading" << out << std::endl;
		std::cout << "reading input parameters: to read " << wave.gcount() << std::endl;
		write = lame_encode_buffer_interleaved(lame, pcm_buffer, wave.gcount(), mp3_buffer, MP3_SIZE);
		//write = lame_encode_buffer(lame, NULL, pcm_buffer, wave.gcount(), mp3_buffer, MP3_SIZE);
		std::cout << "encoding" << out << std::endl;
		mp3.write(reinterpret_cast<char *>(mp3_buffer), write); // a bit more o problems
		std::cout << "saveing" << out << std::endl;
	}
	write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
	mp3.write(reinterpret_cast<char *>(mp3_buffer), write);
	*/
	lame_close(lame);	
}
/*
void convert_STL2()
{	
	std::string in{"./ocean.wav"}, out{"./ocean.mp3"};
	std::ifstream wave(in.c_str(), std::ios_base::binary | std::ios_base::in);
	std::ofstream mp3(out.c_str(), std::ios_base::binary | std::ios_base::out);
	
	if(!wave.is_open() || wave.fail())
	{
		std::cout << "cannot open file: " << in << std::endl;
		return;
	}
	if(!mp3.is_open() || mp3.fail())
	{
		std::cout << "cannot open file: " << out << std::endl;
		return;
	}	
	
	WaveFile wf;
	extract_and_print(wave, wf);	
	lame_t lame = lame_init();
	init_lame_params(lame, wf);
		
 	int size = 1.25 * wf.data.size() + 7200;
	std::vector<unsigned char> mp3Buffer;
	mp3Buffer.resize(size*2);
	
	
	std::cout << "All Set..."<< std::endl;
 	int write{0};
	write = lame_encode_buffer_interleaved(lame, reinterpret_cast<short int*>( &wf.data[0] ), wf.data.size(), &mp3Buffer[0], mp3Buffer.size());
	std::cout << "writing: " << write << std::endl;
	mp3.write(reinterpret_cast<char *>(&mp3Buffer[0]), write);
 	write = lame_encode_flush(lame, &mp3Buffer[0], mp3Buffer.size());
	mp3.write(reinterpret_cast<char *>(&mp3Buffer[0]), write);
 
	lame_close(lame);
}*/

int main(int argc, char**argv )
{	
	lameWrapper::WAVE waveFile;
	lameWrapper::loadWave("./ocean.wav",waveFile);
	
	LameWrapper::Params p;
	if(LameWrapper::parseInput(argc, argv, p) == 0)		
		convert_STL();//convert(p.input.c_str(), p.output.c_str());
	else
		std::cout << "parsing failed" << std::endl;
	
	return 0;
}
