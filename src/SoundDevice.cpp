#include "SoundDevice.h"

#include "Log.h"

#include <AL\alc.h>

SoundDevice::SoundDevice()
{
	SystemLog->Out("Started audio thread", 3);

	p_ALCDevice = alcOpenDevice(nullptr); // default device
	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	alcMakeContextCurrent(p_ALCContext);

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		SystemLog->Out(std::format("Opened device: {}", name), 3);
	}
}

SoundDevice::~SoundDevice() // super important that everything is destroyed here otherwise the DLL won't exit
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_ALCContext);
	alcCloseDevice(p_ALCDevice);
}