#include "SoundBuffer.h"
#include "Log.h"
#include <AL\alext.h>
#include <sndfile.h>

extern Log* SystemLog;

SoundBuffer* SoundBuffer::Get()
{
	static SoundBuffer* sndbuf = new SoundBuffer();
	return sndbuf;
}

ALuint SoundBuffer::AddSoundEffect(const char* filename)
{
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;
	
	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile)
	{
		SystemLog->Out(std::format("Could not open audio in: {} {}", filename, sf_strerror(sndfile)));
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		SystemLog->Out(std::format("Bad sample count in: {} {}", filename, sfinfo.frames));
		sf_close(sndfile);
		return 0;
	}

	// Check format
	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if (sfinfo.channels == 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels == 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!format)
	{
		SystemLog->Out(std::format("Unsupported channel count: {}", sfinfo.channels));
		sf_close(sndfile);
		return 0;
	}

	// Decode audio file into buffer
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		SystemLog->Out(std::format("Failed to read samples in: {} {}", filename, num_frames));
		return 0;
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	// buffer the audio data into a new buffer object, and then free and close the file

	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);

	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		SystemLog->Out(std::format("Open AL Error: {}", alGetString(err)));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	p_SoundEffectBuffers.push_back(buffer); // add to list of known buffers

	return buffer;
}

bool SoundBuffer::RemoveSoundEffect(const ALuint& buffer)
{
	auto it = p_SoundEffectBuffers.begin();
	while (it != p_SoundEffectBuffers.end())
	{
		if (*it == buffer)
		{
			alDeleteBuffers(1, &*it);

			it = p_SoundEffectBuffers.erase(it);

			return true;
		}
		else
		{
			++it;
		}
	}
	return false; // couldn't find to remove
}

SoundBuffer::SoundBuffer()
{
	p_SoundEffectBuffers.clear();
}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());

	p_SoundEffectBuffers.clear();
}