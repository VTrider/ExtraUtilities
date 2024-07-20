#include "SoundDevice.h"
#include "Log.h"
#include <AL\alc.h>

extern Log* SystemLog;

SoundDevice* SoundDevice::Get()
{
	static SoundDevice* sndDevice = new SoundDevice();
	return sndDevice;
}

SoundDevice::SoundDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr); // default device
	if (!p_ALCDevice)
		throw("Failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	if (!p_ALCContext)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))
		throw("Failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		SystemLog->Out("Opened device: " + std::string(name));
	}
}

SoundDevice::~SoundDevice()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_ALCContext);
	alcCloseDevice(p_ALCDevice);
}