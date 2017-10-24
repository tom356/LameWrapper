#include<iostream>
#include "LameWrapper.h"

using namespace lamewrapper;

Mp3Writer::Mp3Writer(){}
Mp3Writer::Mp3Writer(const char *filePath)
{
	open(filePath);
}
Mp3Writer::~Mp3Writer()
{
	close();
}
bool Mp3Writer::open(const char *filePath)
{
	mFile.open(filePath, std::ios_base::binary | std::ios_base::out);
	if(!mFile.is_open() || mFile.fail())
	{
		std::cout << "cannot open file: " << filePath << std::endl;				
		return false;
	}	
	return true;
}
void Mp3Writer::close()
{
	mFile.close();
}
void Mp3Writer::writeBuffer(const char *buffer, int byteCount)
{
	if(mFile.is_open())
		mFile.write(buffer, byteCount);
}
void Mp3Writer::writeBuffer(Mp3Buffer &buffer, int byteCount)
{
	if(mFile.is_open())
		mFile.write(reinterpret_cast<char*>(buffer.data()), byteCount);
}

void Encoder::initLameParams()
{		
	mLame = lame_init();
	int channels{			mWave.getWave().fmt.channels				};
	int data_size{			mWave.getWave().data.size					};
	int bits_per_sample{	mWave.getWave().fmt.bps						};
	int samplerate{			mWave.getWave().fmt.samplerate				};
	int num_samples{		data_size / (channels*bits_per_sample/8)	};
		
    lame_set_num_channels(	mLame, channels );	
	lame_set_in_samplerate(	mLame, samplerate);
	lame_set_num_samples(	mLame, num_samples );
	lame_set_brate(			mLame, 16);   	
	lame_set_quality(		mLame, 2);
	lame_set_mode(			mLame, (channels == 1 ? MONO:STEREO) );		
	lame_set_scale(			mLame, (bits_per_sample == 8 ? 128:1) );
	//lame_set_findReplayGain(lame, 1);
	//lame_set_VBR(lame, vbr_default);
		
	lame_init_params(mLame);		
}
bool Encoder::checkForEncodingErrors(int write)
{
	switch(write)
	{
		case -1:  	std::cout << "mp3buf was too small" << std::endl; 			return false; break;
		case -2:  	std::cout << "malloc() problem" << std::endl; 				return false; break;
		case -3:  	std::cout << "lame_init_params() not called" << std::endl; 	return false; break;
		case -4:  	std::cout << "psycho acoustic problems" << std::endl; 		return false; break;
		default:	std::cout << "encoded: " << write << "bytes" << std::endl; 	return true;  break;
	}	
}
bool Encoder::encode_8bps()
{
	ShortBuffer pcmBuffer;
	mWave.copySamples(pcmBuffer);
	
	// encode
	std::cout << "encoding..." << std::endl;
	
	int write{0};
	if(mWave.getFmt().channels == 1)
	{	
		write = lame_encode_buffer(mLame, 
								   pcmBuffer.data(),
								   NULL,
								   pcmBuffer.size(),
								   mMp3Buffer.data(), 
								   mMp3Buffer.size() );
	}
	else
	{	
		write = lame_encode_buffer_interleaved(mLame, 
											   pcmBuffer.data(), 
											   pcmBuffer.size(),
											   mMp3Buffer.data(), 
											   mMp3Buffer.size() );
	}

	if(!checkForEncodingErrors(write)) 
		return false;
	
	mMp3Writer.writeBuffer(mMp3Buffer, write);
	
	return true;
}
bool Encoder::encode_16bps()
{
	ShortBuffer pcmBuffer;
	mWave.copySamples(pcmBuffer);
	
	// encode
	std::cout << "encoding..." << std::endl;
	
	int write{0};
	if(mWave.getFmt().channels == 1)
	{	
		write = lame_encode_buffer(mLame, 
								   pcmBuffer.data(),
								   NULL,
								   pcmBuffer.size(),
								   mMp3Buffer.data(), 
								   mMp3Buffer.size() );
	}
	else
	{	
		write = lame_encode_buffer_interleaved(mLame, 
											   pcmBuffer.data(), 
											   pcmBuffer.size(),
											   mMp3Buffer.data(), 
											   mMp3Buffer.size() );
	}

	if(!checkForEncodingErrors(write)) 
		return false;
	mMp3Writer.writeBuffer(mMp3Buffer, write);
	
	return true;
}
bool Encoder::encode_24bps()
{
	return false;
}
bool Encoder::encode_32bps()
{
	return false;
}
bool Encoder::encode_64bps()
{
	return false;
}
bool Encoder::flush()
{
	int write{0};
	write = lame_encode_flush(mLame, 
							  mMp3Buffer.data(), 
							  mMp3Buffer.size() );	
	std::cout << "flushed: " << write << "bytes" << std::endl;
	
	mMp3Writer.writeBuffer(mMp3Buffer, write);
	return true;
}
bool Encoder::initOutputBuffer()
{
	mMp3Buffer.clear();
	mMp3Buffer.resize(static_cast<int>(1.25*mWave.getData().size+7200)); // worst case scenario according to lame.h	
	return true;
}
bool Encoder::failedAndCleanup()
{
	cleanup();
	return false;
}
void Encoder::cleanup()
{
	lame_close(mLame);	
	mMp3Writer.close();	
}
bool Encoder::encode(const std::string &in, const std::string &out)
{	
	if(!mWave.load(in.c_str()))
		return failedAndCleanup();	
	mWave.printInfo();
		
	if(!mMp3Writer.open(out.c_str())) 
		return failedAndCleanup();
		
	initLameParams();	
	initOutputBuffer();
			
	// select encoding function base on bits per sample
	switch(mWave.getFmt().bps)
	{
		case 8: if(!encode_8bps())return failedAndCleanup(); break;
		case 16: if(!encode_16bps())return failedAndCleanup(); break;
		case 24: if(!encode_24bps())return failedAndCleanup(); break;
		case 32: if(!encode_32bps())return failedAndCleanup(); break;
		case 64: if(!encode_64bps())return failedAndCleanup(); break;
		default: std::cout<<mWave.getFmt().bps<<" bits per sample, not supported"<<std::endl; return false; break;
	}
	if(!flush())
		return failedAndCleanup();
	std::cout << "Done" << std::endl;
	cleanup();	
	return true;
}
Encoder::Encoder()
{
}
Encoder::~Encoder()
{
}