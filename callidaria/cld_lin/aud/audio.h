#pragma once

#include <iostream>
#include <AL/al.h>
#include <glm/glm.hpp>

class Audio
{
public:
	Audio(const char* path); // standard values = { 1.0f,1.0f,glm::vec3(0.0f),glm::vec3(0.0f),false }
	Audio(const char* path,float gain,float pitch,glm::vec3 pos,glm::vec3 vel,bool play_loop);

	void play();	// plays the sound
	void remove();	// removes the audio entity

	// setters
	void set_all(float gain,float pitch,glm::vec3 pos,glm::vec3 vel,bool play_loop);
	void set_gain(float gain);
	void set_pitch(float pitch);
	void set_position(glm::vec3 pos);
	void set_velocity(glm::vec3 vel);
	void set_loop(bool play_loop); // ??should be possible regardless of alSourcei call
private:
	void read(const char* path); // reads and sets up the audio data
private:
	unsigned int m_buffer,m_audio;	// buffer and audio members
	unsigned char* m_xbuffer; // ??remove this somehow from members
};
