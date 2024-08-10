#pragma once

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>

#include <vector>

class SoundDevice
{
private:
	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

public:
	SoundDevice();
	~SoundDevice();
};