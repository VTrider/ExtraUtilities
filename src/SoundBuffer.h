#pragma once

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>

#include <vector>

class SoundBuffer
{
private:
	ALuint bufferID;

public:

	SoundBuffer(const char* filePath);
	~SoundBuffer();

	ALuint GetID() const { return bufferID; }
};