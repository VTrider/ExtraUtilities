#pragma once

#include <AL\al.h>
#include <vector>

class SoundBuffer
{
private:
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> p_SoundEffectBuffers;	

public:
	static SoundBuffer* Get();

	ALuint AddSoundEffect(const char* filename);
	bool RemoveSoundEffect(const ALuint& buffer);
};