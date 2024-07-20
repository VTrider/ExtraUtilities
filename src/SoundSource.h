#pragma once

#include "AL\al.h"

class SoundSource
{
private:
	ALuint p_Source;
	float p_Pitch = 1.0f;
	float p_Gain = 1.0f;
	float p_Position[3] = { 0, 0, 0 };
	float p_Velocity[3] = { 0, 0, 0 };
	bool p_LoopSound = false;
	ALuint p_Buffer = 0;

public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint buffer_to_play);
};