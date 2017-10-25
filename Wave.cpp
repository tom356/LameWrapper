#include <iostream>
#include <cstring>
#include "Wave.h"

using namespace lamewrapper;

RIFF::RIFF():size(0)
{
	for(int i=0;i<4;i++)
	{
		id[i]=' ';
		format[i]=' ';
	}
}
FMT::FMT():size(0),format_code(0),channels(0),samplerate(0),avg_bps(0),
			block_align(0),bps(0),ext_size(0),valid_bps(0),channel_mask(0)
{
	for(int i=0;i<4;i++)
		id[i]=' ';
	for(int i=0;i<16;i++)
		sub_format[i]=' ';
}
FACT::FACT():size(0),sample_length(0)
{
	for(int i=0;i<4;i++)
		id[i]=' ';
}
DATA::DATA():size(0),padbyte(false)
{	
	for(int i=0;i<4;i++)
		id[i]=' ';
}
WAVE::WAVE()
{

}
bool Wave::getRiffID()
{	
	mFile.read(mWave.riff.id, 4);
	return checkStreamState("cannot read RiffID");
}
bool Wave::getRiffSize()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.riff.size), 4);
	return checkStreamState("cannot read RiffSize");
}
bool Wave::getRiffFormat()
{	
	mFile.read(mWave.riff.format, 4);
	return checkStreamState("cannot read RiffFormat");
}
bool Wave::getFmtID()
{	
	mFile.read(mWave.fmt.id, 4);
	return checkStreamState("cannot read FmtID");
}
bool Wave::getFmtSize()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.size), 4);
	return checkStreamState("cannot read FmtSize");
}
bool Wave::getFmtAudioFormat()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.format_code), 2);
	return checkStreamState("cannot read FmtAudioFormat");
}
bool Wave::getFmtNumChannels()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.channels), 2);
	return checkStreamState("cannot read FmtNumChannels");
}
bool Wave::getFmtSampleRate()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.samplerate), 4);
	return checkStreamState("cannot read FmtSampleRate");
}
bool Wave::getFmtAvgBps()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.avg_bps), 4);
	return checkStreamState("cannot read FmtAvgBps");
}
bool Wave::getFmtBlockAlign()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.block_align), 2);
	return checkStreamState("cannot read FmtBlockAlign");
}
bool Wave::getFmtBps()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.bps), 2);
	return checkStreamState("cannot read FmtBps");
}
bool Wave::getFmtExtSize() 		// for non pcm-data, for extensible format
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.ext_size), 2);
	return checkStreamState("cannot read FmtExtSize");
}
bool Wave::getFmtValidBPS() 		// for extensible format
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.valid_bps), 2);
	return checkStreamState("cannot read FmtValidBps");
}
bool Wave::getFmtChannelMask() 	// for extensible format
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fmt.channel_mask), 4);
	return checkStreamState("cannot read FmtChannelMask");
}
bool Wave::getFmtSubFormat()		// for extensible format
{	
	mFile.read(mWave.fmt.sub_format, 16);
	return checkStreamState("cannot read FmtSubFormat");
}
bool Wave::getFactID()			// for non pcm-data, for extensible format
{	
	mFile.read(mWave.fact.id, 4);
	return checkStreamState("cannot read FactID");
}	
bool Wave::getFactSize()			// for non pcm-data, for extensible format
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fact.size), 4);
	return checkStreamState("cannot read FactSize");
}	
bool Wave::getFactSampleLength()	// for non pcm-data, for extensible format
{	
	mFile.read(reinterpret_cast<char*>(&mWave.fact.sample_length), 4);
	return checkStreamState("cannot read FactSampleLength");
}
bool Wave::getDataID()
{	
	mFile.read(mWave.data.id, 4);
	return checkStreamState("cannot read DataID");
}
bool Wave::getDataSize()
{	
	mFile.read(reinterpret_cast<char*>(&mWave.data.size), 4);
	return checkStreamState("cannot read DataSize");
}
bool Wave::getDataSamples()
{	
	mWave.data.samples.clear();
	mWave.data.samples.reserve(mWave.data.size);
	mFile.read(reinterpret_cast<char*>(&mWave.data.samples[0]), mWave.data.size);
	return checkStreamState("cannot read DataSamples");
}
bool Wave::getPadbyte()
{
	return (mWave.data.size%2 != 0);
}
bool Wave::checkStreamState(const char* errMessage)
{
	if(mFile.fail()) 
	{
		std::cout << errMessage << std::endl;
		mFile.close();
		return false;
	}
	return true;
}
std::string Wave::getFormat(WAVEFormatCode format)
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
bool Wave::load(const char* wavePath)
{
	mFile.open(wavePath, std::ios_base::binary | std::ios_base::in);	
	if(!mFile.is_open())
	{
		std::cout << "cannot open file: " << wavePath << std::endl; 
		return false;
	}
			
	if(!getRiffID()			)return false;
	if(!getRiffSize()		)return false;
	if(!getRiffFormat()		)return false;
	if(!getFmtID()			)return false;
	if(!getFmtSize()		)return false;
	if(!getFmtAudioFormat()	)return false;
	if(!getFmtNumChannels()	)return false;
	if(!getFmtSampleRate()	)return false;
	if(!getFmtAvgBps()		)return false;
	if(!getFmtBlockAlign()	)return false;
	if(!getFmtBps()			)return false;
	
	// non-pcm
	if(mWave.fmt.size == 18)
	{
		if(!getFmtExtSize()			)return false;
		
		if(!getFactID()				)return false;
		if(!getFactSize()			)return false;
		if(!getFactSampleLength()	)return false;
	}	
	// extensible format
	if(mWave.fmt.size > 18)
	{
		if(!getFmtExtSize()			)return false;
		if(!getFmtValidBPS() 		)return false;
		if(!getFmtChannelMask()		)return false;
		if(!getFmtSubFormat()		)return false;
		
		if(!getFactID()				)return false;
		if(!getFactSize()			)return false;
		if(!getFactSampleLength()	)return false;	
	}
	
	if(!getDataID()			)return false;
	if(!getDataSize()		)return false;
	if(!getDataSamples()	)return false;
	
	mFile.close();
	return true;
}
void Wave::printInfo()
{
	std::cout << "--------------Wave info------------------"<<std::endl;	
	std::cout << "RiffID: "<< 			mWave.riff.id[0] << mWave.riff.id[1] << mWave.riff.id[2] << mWave.riff.id[3] << std::endl;
	std::cout << "RiffSize: "<< 		mWave.riff.size << std::endl;
	std::cout << "RiffFormat: "<< 		mWave.riff.format[0] << mWave.riff.format[1] << mWave.riff.format[2] << mWave.riff.format[3] << std::endl;
	std::cout << "FmtID: "<< 			mWave.fmt.id[0] << mWave.fmt.id[1] << mWave.fmt.id[2] << mWave.fmt.id[3] << std::endl;
	std::cout << "FmtSize: "<< 			mWave.fmt.size << std::endl;	
	std::cout << "FmtAudioFormat: "<< 	getFormat(static_cast<WAVEFormatCode>(mWave.fmt.format_code)) << std::endl;	
	std::cout << "FmtNumChannels: "<< 	mWave.fmt.channels << std::endl;
	std::cout << "FmtSampleRate: "<< 	mWave.fmt.samplerate << std::endl;
	std::cout << "FmtAvgBps: "<< 		mWave.fmt.avg_bps << std::endl;
	std::cout << "FmtBlockAlign: "<< 	mWave.fmt.block_align << std::endl;
	std::cout << "FmtBps: "<< 			mWave.fmt.bps << std::endl;
	// non PCM
	if(mWave.fmt.size == 18)
	{
		std::cout << "FmtExtSize: "<<		mWave.fmt.ext_size << std::endl;
		std::cout << "FactID: "<< 			mWave.fact.id[0] << mWave.fact.id[1] << mWave.fact.id[2] << mWave.fact.id[3] << std::endl;
		std::cout << "FactSize: "<< 		mWave.fact.size << std::endl;
		std::cout << "FactSampleLength: "<< mWave.fact.sample_length << std::endl;		
	}	
	// extensible format
	if(mWave.fmt.size > 18)
	{
		std::cout << "FmtExtSize: "<<		mWave.fmt.ext_size << std::endl;
		std::cout << "FmtValidBPS: "<<		mWave.fmt.valid_bps << std::endl;
		std::cout << "FmtChannelMask: "<<	mWave.fmt.channel_mask << std::endl;
		std::cout << "FmtExtSize: ";
		for(int i=0;i<16;i++)
			std::cout << mWave.fmt.sub_format[i];
		std::cout << std::endl;		
		std::cout << "FactID: "<< 			mWave.fact.id[0] << mWave.fact.id[1] << mWave.fact.id[2] << mWave.fact.id[3] << std::endl;
		std::cout << "FactSize: "<< 		mWave.fact.size << std::endl;
		std::cout << "FactSampleLength: "<< mWave.fact.sample_length << std::endl;		
	}		
	std::cout << "DataId: "<< 			mWave.data.id[0] << mWave.data.id[1] << mWave.data.id[2] << mWave.data.id[3] << std::endl;
	std::cout << "DataSize: "<< 		mWave.data.size << std::endl;
	std::cout << "-----------------------------------------"<< std::endl;
	std::cout << std::endl;		
}
const WAVE& Wave::getWave()
{
	return mWave;
}
const RIFF& Wave::getRiff()
{
	return mWave.riff;
}
const FMT& Wave::getFmt()
{
	return mWave.fmt;
}
const FACT& Wave::getFact()
{
	return mWave.fact;
}
const DATA& Wave::getData()
{
	return mWave.data;
}
void Wave::copySamples(std::vector<short int> &samples)
{
	if(mWave.fmt.bps == 8)
	{
		samples.resize(mWave.data.size);		
		for(int i=0;i<mWave.data.size;i++)
			samples[i] = static_cast<short int>(static_cast<unsigned char>(mWave.data.samples[i]));
	}
	if(mWave.fmt.bps == 16)
	{		
		samples.resize(mWave.data.size/2);
		memcpy(samples.data(), mWave.data.samples.data(), mWave.data.size);
//		for(int i=0;i<mWave.data.size/2;i++)
//			samples[i] = static_cast<short int>(static_cast<unsigned char>(mWave.data.samples[2*i])|
//								static_cast<unsigned char>(mWave.data.samples[2*i+1])<<8 );
	}
}
Wave::Wave()
{
		
}
Wave::Wave(const char* wavePath)
{
	load(wavePath);
}
