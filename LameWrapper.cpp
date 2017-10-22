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

int16_t lameWrapper::endianSwap(int16_t i)
{
	return (i&0xff)<<8 | (i&0xff00)>>8;
}
int32_t lameWrapper::endianSwap(int32_t i)
{
	return (i&0xff)<<24 | (i&0xff00)<<8 | (i&0xff0000)>>8 | (i&0xff000000)>>24;
}
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
	data.samples.resize(data.size);
	file.read(reinterpret_cast<char*>(&data.samples[0]), data.size);
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
	   
	file.close();	
	return true;
}
std::string getFormat(WAVEFormatCode format)
{
	switch(format)
	{
		case WAVE_FORMAT_PCM: 			return "PCM"; break;
		case WAVE_FORMAT_IEEE_FLOAT: 	return "IEEE_FLOAT"; break;
		case WAVE_FORMAT_ALAW: 			return "ALAW"; break;
		case WAVE_FORMAT_MULAW: 		return "MULAW"; break;
		case WAVE_FORMAT_EXTANSIBLE: 	return "EXTANSIBLE"; break;
		default: 						return "Unknown"; break;
	}	
}
void lameWrapper::printWaveInfo(WAVE &wf)
{
	std::cout << "--------------Wave info------------------"<<std::endl;
	std::cout << "RiffID: "<< wf.riff.id[0] << wf.riff.id[1] << wf.riff.id[2] << wf.riff.id[3] << std::endl;
	std::cout << "RiffSize: "<< wf.riff.size << std::endl;
	std::cout << "RiffFormat: "<< wf.riff.format[0] << wf.riff.format[1] << wf.riff.format[2] << wf.riff.format[3] << std::endl;
	std::cout << "FmtID: "<< wf.fmt.id[0] << wf.fmt.id[1] << wf.fmt.id[2] << wf.fmt.id[3] << std::endl;
	std::cout << "FmtSize: "<< wf.fmt.size << std::endl;	
	std::cout << "FmtAudioFormat: "<< getFormat(static_cast<WAVEFormatCode>(wf.fmt.formatcode)) << std::endl;	
	std::cout << "FmtNumChannels: "<< wf.fmt.channels << std::endl;
	std::cout << "FmtSampleRate: "<< wf.fmt.samplerate << std::endl;
	std::cout << "FmtAvgBps: "<< wf.fmt.avgbps << std::endl;
	std::cout << "FmtBlockAlign: "<< wf.fmt.blockalign << std::endl;
	std::cout << "FmtBps: "<< wf.fmt.bps << std::endl;
	std::cout << "DataId: "<< wf.data.id[0] << wf.data.id[1] << wf.data.id[2] << wf.data.id[3] << std::endl;
	std::cout << "DataSize: "<< wf.data.size << std::endl;
	std::cout << "-----------------------------------------"<< std::endl;
	std::cout << std::endl;
	/*
	std::cout << "DataSamples(some): ";
	int max = (wf.data.size>350?350:wf.data.size);
	for(int i=0;i<max;i++)		
		std::cout << wf.data.samples.at(i)<<"  ";
	std::cout << std::endl; */
}
void lameWrapper::init_lame_params(lame_t &lame, WAVE &wf)
{	
	int channels{			wf.fmt.channels								};
	int data_size{			wf.data.size								};
	int bits_per_sample{	wf.fmt.bps									};
	int samplerate{			wf.fmt.samplerate							};
	int num_samples{		data_size / (channels*bits_per_sample/8)	};
		
    lame_set_num_channels(	lame, channels );	
	lame_set_in_samplerate(	lame, samplerate);
	lame_set_num_samples(	lame, num_samples );
	lame_set_brate(			lame, 16);   	
	lame_set_quality(		lame, 2);
	lame_set_mode(			lame, (channels == 1 ? MONO:STEREO) );		
	lame_set_scale(			lame, (bits_per_sample == 8 ? 128:1) );
	//lame_set_findReplayGain(lame, 1);
	//lame_set_VBR(lame, vbr_default);
		
	lame_init_params(lame);		
}
bool lameWrapper::encode_8bps(lame_t &lame, WAVE &wave_file, std::vector<unsigned char> &mp3_buffer, std::ofstream &mp3File)
{
	// init pcm buffer
	std::vector<short int> pcm_buffer;
	pcm_buffer.resize(wave_file.data.size);
	for(int i=0;i<wave_file.data.size;i++)
		pcm_buffer[i] = static_cast<short int>(wave_file.data.samples[i]);
		
	// encode
	std::cout << "encoding..." << std::endl;
	int write{0};
	if(wave_file.fmt.channels == 1)
	{	
		write = lame_encode_buffer(lame, 
								   &pcm_buffer[0],//reinterpret_cast<short int*>( &wave_file.data.samples[0] ), 
								   NULL,
								   pcm_buffer.size(),//wave_file.data.samples.size(),
								   &mp3_buffer[0], 
								   mp3_buffer.size() );
	}
	else
	{	
		write = lame_encode_buffer_interleaved(lame, 
											   &pcm_buffer[0], 
											   pcm_buffer.size(),
											   &mp3_buffer[0], 
											   mp3_buffer.size() );
	}
	switch(write)
	{
		case -1:  	std::cout << "mp3buf was too small" << std::endl; 			return false; break;
		case -2:  	std::cout << "malloc() problem" << std::endl; 				return false; break;
		case -3:  	std::cout << "lame_init_params() not called" << std::endl; 	return false; break;
		case -4:  	std::cout << "psycho acoustic problems" << std::endl; 		return false; break;
		default:	std::cout << "encoded: " << write << "bytes" << std::endl; 	break;
	}	
	
	// write data
	mp3File.write(reinterpret_cast<char *>(&mp3_buffer[0]), write);		
	
	return true;
}
bool lameWrapper::encode_16bps(lame_t &lame, WAVE &wave_file, std::vector<unsigned char> &mp3_buffer, std::ofstream &mp3File)
{
	// encode
	std::cout << "encoding..." << std::endl;
	int write{0};
	if(wave_file.fmt.channels == 1)
	{	
		write = lame_encode_buffer(lame, 
								   reinterpret_cast<short int*>( &wave_file.data.samples[0] ), 
								   NULL,
								   wave_file.data.size,
								   &mp3_buffer[0], 
								   mp3_buffer.size() );
	}
	else
	{	
		write = lame_encode_buffer_interleaved(lame, 
											   reinterpret_cast<short int*>( &wave_file.data.samples[0] ), 
											   wave_file.data.size,
											   &mp3_buffer[0], 
											   mp3_buffer.size() );
	}
	switch(write)
	{
		case -1:  	std::cout << "mp3buf was too small" << std::endl; 			return false; break;
		case -2:  	std::cout << "malloc() problem" << std::endl; 				return false; break;
		case -3:  	std::cout << "lame_init_params() not called" << std::endl; 	return false; break;
		case -4:  	std::cout << "psycho acoustic problems" << std::endl; 		return false; break;
		default:	std::cout << "encoded: " << write << "bytes" << std::endl; 	break;
	}	
	
	// write data
	mp3File.write(reinterpret_cast<char *>(&mp3_buffer[0]), write);		
	
	return true;
}
bool lameWrapper::waveToMp3(const std::string &in, const std::string &out)
{
	// init wave
	WAVE wave_file;
	if(!loadWave(in, wave_file)) 
		return false;
	
	printWaveInfo(wave_file);
	
	// init ofstream
	std::ofstream mp3File(out.c_str(), std::ios_base::binary | std::ios_base::out);
	if(!mp3File.is_open() || mp3File.fail())
	{
		std::cout << "cannot open file: " << out << std::endl;
		return false;
	}
	
	//init lame
	lame_t lame = lame_init();
	init_lame_params(lame, wave_file);
	
	// init mp3 buffer
	std::vector<unsigned char> mp3_buffer;
	mp3_buffer.resize(static_cast<int>(1.25*wave_file.data.size+7200)); // worst case scenario according to lame.h
			
	switch(wave_file.fmt.bps)
	{
		case 8: encode_8bps(lame, wave_file, mp3_buffer, mp3File); break;
		case 16: encode_8bps(lame, wave_file, mp3_buffer, mp3File); break;
		default: std::cout<<wave_file.fmt.bps<<" bits per sample, not supported"<<std::endl; return false; break;
	}
	
	// encode flush
	int write{0};
	write = lame_encode_flush(lame, 
							  &mp3_buffer[0], 
							  mp3_buffer.size() );	
	std::cout << "flushed: " << write << "bytes" << std::endl;
	
	// write data
	mp3File.write(reinterpret_cast<char *>(&mp3_buffer[0]), write);		
	std::cout << "Done" << std::endl;
	
	std::cout << "Replay gain:" << lame_get_findReplayGain(lame) << std::endl;
		
	lame_close(lame);
	
	mp3File.close();
	
	return true;
}