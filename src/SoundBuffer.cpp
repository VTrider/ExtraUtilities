#include "SoundBuffer.h"

#include "Log.h"

#include <AL\alext.h>
#include <sndfile.h>

SoundBuffer::SoundBuffer(const char* filePath)
{
	SF_INFO fileInfo;
	SNDFILE* sndFile = sf_open(filePath, SFM_READ, &fileInfo);

	if (!sndFile)
	{
		SystemLog->Out("Failed to open sound file", 1);
		return;
	}

	std::vector<short> samples(fileInfo.frames * fileInfo.channels);
	sf_read_short(sndFile, samples.data(), samples.size());
	sf_close(sndFile);

	alGenBuffers(1, &bufferID);

	ALenum format;

	if (fileInfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (fileInfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else
	{
		SystemLog->Out("Unsupported channel count", 1);
		return;
	}

	alBufferData(bufferID, format, samples.data(), samples.size() * sizeof(short), fileInfo.samplerate);
}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(1, &bufferID);
}