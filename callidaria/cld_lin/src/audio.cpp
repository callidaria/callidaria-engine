#include "../aud/audio.h"

Audio::Audio(const char* path) { Audio(path,1.0f,1.0f,glm::vec3(0.0f),glm::vec3(0.0f),false); }
Audio::Audio(const char* path,float gain,float pitch,glm::vec3 pos,glm::vec3 vel,bool play_loop)
{
	read(path);

	// setting audio parameters
	alSourcei(m_audio,AL_BUFFER,m_buffer);
	set_all(gain,pitch,pos,vel,play_loop);
}
void Audio::play() { alSourcePlay(m_audio); }
void Audio::remove()
{
	delete[] m_xbuffer; // ??can this be done earlier to save member variable
	alDeleteSources(1,&m_audio); 
	alDeleteBuffers(1,&m_buffer);
}
void Audio::set_all(float gain,float pitch,glm::vec3 pos,glm::vec3 vel,bool play_loop)
{
	set_gain(gain);
	set_pitch(pitch);
	set_position(pos);
	set_velocity(vel);
	set_loop(play_loop);
}
void Audio::set_gain(float gain) { alSourcef(m_audio,AL_GAIN,gain); }
void Audio::set_pitch(float pitch) { alSourcef(m_audio,AL_PITCH,pitch); }
void Audio::set_position(glm::vec3 pos) { alSource3f(m_audio,AL_POSITION,pos.x,pos.y,pos.z); }
void Audio::set_velocity(glm::vec3 vel) { alSource3f(m_audio,AL_VELOCITY,vel.x,vel.y,vel.z); }
void Audio::set_loop(bool play_loop) { alSourcei(m_audio,AL_LOOPING,play_loop); }
void Audio::read(const char* path)
{
	// buffer setup
	alGenBuffers(1,&m_buffer);
	alGenSources(1,&m_audio);

	// some variables
	FILE* file = fopen(path, "rb"); // see if file exists
	char type[4];
	unsigned long size,cSize,rate,abps,dSize;
	short fType,ch,bps,bips;

	// read audio
	fread(type,sizeof(char),4,file);
	fread(&size,sizeof(char),4,file);
	fread(type,sizeof(char),4,file);
	fread(type,sizeof(char),4,file);
	fread(&cSize,sizeof(char),4,file);
	fread(&fType,sizeof(short),1,file);
	fread(&ch,sizeof(short),1,file);
	fread(&rate,sizeof(char),4,file);
	fread(&abps,sizeof(char),4,file);
	fread(&bps,sizeof(short),1,file);
	fread(&bips,sizeof(short),1,file);
	fread(type,sizeof(char),4,file);
	fread(&dSize,sizeof(char),4,file);

	// why the fuck you don't just set this variable gawddammit. no fucking reason!!
	unsigned char* xbuffer = new unsigned char[dSize];
	fread(xbuffer,sizeof(unsigned char),dSize,file);
	fclose(file);

	// ??will removing this cause a mem leak ...and above all else, do i care?
	// !!it even causes a mem leak in it's current state. this xbuffer management is a disaster
	m_xbuffer = xbuffer; // some (?unnessessary) and very verbose high level retardation. kill it with fire!

	// audio buffer data
	ALenum format;
	if (bips == 8) {
		if (ch == 1) format = AL_FORMAT_MONO8;
		else if (ch == 2) format = AL_FORMAT_STEREO8;
	} else if (bips == 16) {
		if (ch == 1) format = AL_FORMAT_MONO16;
		else if (ch == 2) format = AL_FORMAT_STEREO16;
	} else std::cout << "invalid bits per second value" << std::endl; // formatting output
	alBufferData(m_buffer,format,m_xbuffer,dSize,rate);
	//delete[] xbuffer;
}
