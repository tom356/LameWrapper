#include<iostream>
#include "LameWrapper.h"
using namespace lameWrapper;

RIFF::RIFF():size(0)
{
	for(int i=0;i<4;++i) id[i]=' ';
	for(int i=0;i<4;++i) format[i]=' ';
}
FMT::FMT():size(0),formatcode(0),channels(0),samplerate(0),avgbps(0),blockalign(0),bps(0)
{
	for(int i=0;i<4;++i) id[i]=' ';
}
DATA::DATA():size()
{
	for(int i=0;i<4;++i) id[i]=' ';
}
WAVE::WAVE(){}

bool lameWrapper::getRiffID(std::ifstream &file, RIFF &riff)
{
	file.read(reinterpret_cast<char*>(&riff.id),4);
	return !file.fail();
}
bool lameWrapper::getRiffSize(std::ifstream &file, RIFF &riff)
{
	file.read(reinterpret_cast<char*>(&riff.size), 4);
	return !file.fail();
}
bool lameWrapper::getRiffFormat(std::ifstream &file, RIFF &riff)
{
	file.read(reinterpret_cast<char*>(&riff.format), 4);
	return !file.fail();
}
bool lameWrapper::getFmtID(std::ifstream &file, FMT &fmt)
{
	file.read(reinterpret_cast<char*>(&fmt.id),4);
	return !file.fail();
}
bool lameWrapper::getFmtSize(std::ifstream &file, FMT &fmt)
{
	file.read(reinterpret_cast<char*>(&fmt.size),4);	
	return !file.fail();
}
bool lameWrapper::getFmtAudioFormat(std::ifstream &file, FMT &fmt)
{
	file.read(reinterpret_cast<char*>(&fmt.formatcode),2);	
	return !file.fail();
}
bool lameWrapper::getFmtNumChannels(std::ifstream &file, FMT &fmt)
{
	file.read(reinterpret_cast<char*>(&fmt.channels),2);	
	return !file.fail();
}
bool lameWrapper::getFmtSampleRate(std::ifstream &file, FMT &fmt)
{
	file.read(reinterpret_cast<char*>(&fmt.samplerate),4);	
	return !file.fail();
}
bool lameWrapper::getFmtAvgBps(std::ifstream &file, FMT &fmt)
{
	file.read(reinterpret_cast<char*>(&fmt.avgbps),4);	
	return !file.fail();
}
bool lameWrapper::getFmtBlockAlign(std::ifstream &file, FMT &fmt)
{
	file.read(reinterpret_cast<char*>(&fmt.blockalign),2);	
	return !file.fail();
}
bool lameWrapper::getFmtBps(std::ifstream &file, FMT &fmt)
{
	file.read(reinterpret_cast<char*>(&fmt.bps),2);	
	return !file.fail();
}
bool lameWrapper::getDataID(std::ifstream &file, DATA &data)
{
	file.read(reinterpret_cast<char*>(&data.id),4);	
	return !file.fail();
}
bool lameWrapper::getDataSize(std::ifstream &file, DATA &data)
{
	file.read(reinterpret_cast<char*>(&data.size),4);
	return !file.fail();
}
bool lameWrapper::getDataSamples(std::ifstream &file, DATA &data)
{	
	data.data.resize(data.size);
	file.read(reinterpret_cast<char*>(&data.data[0]), data.size);	
	return !file.fail();
}
bool lameWrapper::loadWave(const std::string &filePath, WAVE &wf)
{
	std::ifstream file(filePath.c_str(), std::ios_base::binary | std::ios_base::in);
	
	if(!file.is_open()){std::cout << "cannot openFile: " << filePath << std::endl; return false;}
	
	if(!getRiffID(file, wf.riff)){			std::cout << "cannot read RiffID" << std::endl; return false;}
	if(!getRiffSize(file, wf.riff)){		std::cout << "cannot read RiffSize" << std::endl; return false;}
 	if(!getRiffFormat(file, wf.riff)){		std::cout << "cannot read RiffFormat" << std::endl; return false;}
 	if(!getFmtID(file, wf.fmt)){			std::cout << "cannot read FmtID" << std::endl; return false;}
 	if(!getFmtSize(file, wf.fmt)){			std::cout << "cannot read FmtSize" << std::endl; return false;}
 	if(!getFmtAudioFormat(file, wf.fmt)){	std::cout << "cannot read FmtAudioFormat" << std::endl; return false;}
 	if(!getFmtNumChannels(file, wf.fmt)){	std::cout << "cannot read FmtNumChannels" << std::endl; return false;}
 	if(!getFmtSampleRate(file, wf.fmt)){	std::cout << "cannot read FmtSampleRate" << std::endl; return false;}
 	if(!getFmtAvgBps(file, wf.fmt)){		std::cout << "cannot read FmtAvgBps" << std::endl; return false;}
 	if(!getFmtBlockAlign(file, wf.fmt)){	std::cout << "cannot read FmtBlockAlign" << std::endl; return false;}
 	if(!getFmtBps(file, wf.fmt)){			std::cout << "cannot read FmtBps" << std::endl; return false;}
 	if(!getDataID(file, wf.data)){			std::cout << "cannot read DataId" << std::endl; return false;}
 	if(!getDataSize(file, wf.data)){		std::cout << "cannot read DataSize" << std::endl; return false;}
 	if(!getDataSamples(file, wf.data)){		std::cout << "cannot read DataSamples" << std::endl; return false;}
	   
	return true;
}

void lameWrapper::init_lame_params(lame_t &lame, WAWE &wf)
{
	
	lame_set_in_samplerate(lame, wf.fmt.samplerate);
	lame_set_num_samples(lame, wf.data.data.size());
    lame_set_brate(lame, wf.fmt.samplerate * wf.fmt.bps * wf.fmt.channels );
	lame_set_findReplayGain(lame, 1);
	//lame_set_brate(lame, wf.avgbps);
    lame_set_num_channels(lame, wf.fmt.channels );	
	if(wf.fmt.channels == 1) lame_set_mode(lame, MONO );	
	lame_set_VBR(lame, vbr_default);
	lame_set_quality(lame, 0);
	lame_init_params(lame);	
}

bool lameWrapper::waveToMp3()
{
	std::string in{"./ocean.wav"}, out{"./ocean.mp3"};
	std::ofstream mp3(out.c_str(), std::ios_base::binary | std::ios_base::in);
	
	WAWE wf;
	loadWave(in, wf);
	
	lame_t lame = lame_init();
	init_lame_params(lame, wf);
		
	const int PCM_SIZE = 8192; // how much bytes to read at one time
    const int MP3_SIZE = 8192; // how much bytes to read at one time
    unsigned char mp3_buffer[MP3_SIZE];	
	
	std::cout << "All Set..." << out << std::endl;
	
	int write{0},offset{0},maxread{0};
	while(offset < wf.data.data.size())
	{
		maxread = (offset + PCM_SIZE < wf.data.data.size()?PCM_SIZE:wf.data.data.size()-offset);
		
		std::cout << "Reading: " << maxread <<" at offset:" << offset << std::endl;		
		
		//write = lame_encode_buffer(lame, reinterpret_cast<short int*>( &wf.data[offset] ), NULL, maxread, mp3_buffer, MP3_SIZE);				
		write = lame_encode_buffer_interleaved(lame, reinterpret_cast<short int*>( &wf.data.data[offset] ), maxread, mp3_buffer, MP3_SIZE);				
		
		std::cout << "writing: " << write << std::endl;
		
		mp3.write(reinterpret_cast<char *>(mp3_buffer), write); // a bit more o problems		
		
		offset+=maxread;
	}
	write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
	std::cout << "flushing, writing: " << write << std::endl;
	mp3.write(reinterpret_cast<char *>(mp3_buffer), write);
	std::cout << "Done" << out << std::endl; 
}