#pragma once

#include <iostream>
#include <glm/glm.hpp>

class Audio
{
public:
	Audio(const char* path,float gain,float pitch,glm::vec3 pos,glm::vec3 vel,bool loop)
	{
		// SETUP
		alGenBuffers(1,&buffer);
		alGenSources(1,&audio);

		// READ AUDIO
		FILE* file=fopen(path,"rb");
		char type[4];
		unsigned long size,chunkSize,sampleRate,avgBytesPerSec,dataSize;
		short formatType,channels,bytesPerSample,bitsPerSample;
		fread(type,sizeof(char),4,file);
		fread(&size,sizeof(char),4,file);
		fread(type,sizeof(char),4,file);
		fread(type,sizeof(char),4,file);
		fread(&chunkSize,sizeof(char),4,file);
		fread(&formatType,sizeof(short),1,file);
		fread(&channels,sizeof(short),1,file);
		fread(&sampleRate,sizeof(char),4,file);
		fread(&avgBytesPerSec,sizeof(char),4,file);
		fread(&bytesPerSample,sizeof(short),1,file);
		fread(&bitsPerSample,sizeof(short),1,file);
		fread(type,sizeof(char),4,file);
		fread(&dataSize,sizeof(char),4,file);
		xbuffer=new unsigned char[dataSize];
		fread(xbuffer,sizeof(unsigned char),dataSize,file);
		fclose(file);

		// BUFFER
		ALenum format;
		if (bitsPerSample==8) {
			if (channels==1) format=AL_FORMAT_MONO8;
			else if (channels==2) format=AL_FORMAT_STEREO8;
		} else if (bitsPerSample==16) {
			if (channels==1) format=AL_FORMAT_MONO16;
			else if (channels==2) format=AL_FORMAT_STEREO16;
		} alBufferData(buffer,format,xbuffer,dataSize,sampleRate);

		// SOURCE
		alSourcei(audio,AL_BUFFER,buffer);
		alSourcef(audio,AL_GAIN,gain);
		alSourcef(audio,AL_PITCH,pitch);
		alSource3f(audio,AL_POSITION,pos.x,pos.y,pos.z);
		alSource3f(audio,AL_VELOCITY,vel.x,vel.y,vel.z);
		alSourcei(audio,AL_LOOPING,loop);
	}
	void play() { alSourcePlay(audio); }
	void remove()
	{
		delete[] xbuffer;
		alDeleteSources(1,&audio);
		alDeleteBuffers(1,&buffer);
	}
private:
	unsigned int buffer,audio;
	unsigned char* xbuffer;
};
