#pragma once

#include "SoundBuffer.h"

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>

#include <vector>

class SoundSource
{
private:
	ALuint sourceID;

public:

	SoundSource();
	~SoundSource();

	static inline std::vector<ALuint> sourceList;
	ALuint GetID() const { return sourceID; }

	void Play(SoundBuffer& buffer) const;

};