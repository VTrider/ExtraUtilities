#include "SoundSource.h"

#include "Log.h"

#include <vector>

SoundSource::~SoundSource()
{
	alDeleteSources(1, &sourceID);
}

void SoundSource::Play(SoundBuffer& buffer) const
{
	alSourcei(sourceID, AL_BUFFER, buffer.GetID());
	alSourcef(sourceID, AL_GAIN, 0.5f);
	alSourcePlay(sourceID);
	sourceList.push_back(sourceID);
}