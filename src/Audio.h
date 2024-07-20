#pragma once

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>

class Audio
{
private:
	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

public:
	Audio();
	~Audio();


};

