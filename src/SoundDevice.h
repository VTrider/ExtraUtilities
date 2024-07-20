#pragma once

#include <AL\alc.h>

class SoundDevice
{
private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

public:
	static SoundDevice* Get();

};
