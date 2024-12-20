/* Copyright (C) 2023-2024 VTrider
 *
 * This file is part of Extra Utilities.
 *
 * Extra Utilities is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Audio.h"

#include "Log.h"

#include <sndfile.h>

ALuint Audio::GetSource()
{
	ALuint source;
	alGenSources(1, &source);
	activeSources.emplace_back(source);
	return source;
}

ALuint Audio::MakeBuffer(const std::string& filePath)
{
	ALuint bufferID;
	SF_INFO fileInfo;
	SNDFILE* sndFile = sf_open(filePath.c_str(), SFM_READ, &fileInfo);

	if (!sndFile)
	{
		return 0;
	}

	std::vector<short> samples(static_cast<size_t>(fileInfo.frames) * static_cast<size_t>(fileInfo.channels));
	sf_read_short(sndFile, samples.data(), samples.size());
	sf_close(sndFile);

	alGenBuffers(1, &bufferID);

	ALenum format;

	if (fileInfo.channels == 1)
	{
		format = AL_FORMAT_MONO16;
	}
	else if (fileInfo.channels == 2)
	{
		format = AL_FORMAT_STEREO16;
	}
	else
	{
		return 0;
	}

	alBufferData(bufferID, format, samples.data(), samples.size() * sizeof(short), fileInfo.samplerate);

	buffers.emplace(filePath, BufferData{ bufferID, format });
	return bufferID;
}

void Audio::SendSoundRequest(Request request)
{
	requestLock.lock();
	requestQueue.push(request);
	requestLock.unlock();
}

// not the prettiest but it works
bool Audio::IsMono(ALuint source)
{
	for (const auto& [path, bufferData] : buffers)
	{
		if (bufferData.id == source)
		{
			return (bufferData.format == AL_FORMAT_MONO16) ? true : false;
		}
	}
	return false;
}

void Audio::CleanSources()
{
	requestLock.lock();
	ALint state;
	for (size_t i = 0; i < activeSources.size(); i++)
	{
		alGetSourcei(activeSources[i], AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED)
		{
			alDeleteSources(1, &activeSources[i]);
			activeSources.erase(activeSources.begin() + i);
		}
	}
	requestLock.unlock();
}

void Audio::ProcessSoundRequests()
{
	requestLock.lock();
	if (requestQueue.empty())
	{
		requestLock.unlock();
		return;
	}

	ALuint buffer;

	// If the buffer already exists ie. you already loaded that sound file
	// into memory then use it, otherwise make a new one
	auto it = buffers.find(requestQueue.front().filePath);
	if (it != buffers.end())
	{
		buffer = it->second.id;
	}
	else
	{
		buffer = MakeBuffer(requestQueue.front().filePath);
	}

	ALuint sourceToPlay = requestQueue.front().source;
	
	requestQueue.pop();
	activeSources.push_back(sourceToPlay);
	requestLock.unlock();

	alSourcei(sourceToPlay, AL_BUFFER, buffer);
	alSourcePlay(sourceToPlay);
}

ALuint Audio::PlaySoundEffect(const std::string& filePath)
{
	ALuint source = GetSource();
	Request soundRequest = { source, filePath };
	SendSoundRequest(soundRequest);
	return source;
}

float Audio::GetMainVolume()
{
	float gain;
	alGetListenerf(AL_GAIN, &gain);
	return gain;
}

void Audio::SetMainVolume(float gain)
{
	alListenerf(AL_GAIN, gain);
}

float Audio::GetVolume(ALuint source)
{
	float gain;
	alGetSourcef(source, AL_GAIN, &gain);
	return gain;
}

void Audio::SetVolume(ALuint source, float gain)
{
	alSourcef(source, AL_GAIN, gain);
}

bool Audio::GetPaused(ALuint source)
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return (state == AL_PAUSED) ? true : false;
}

void Audio::SetPaused(ALuint source, bool paused)
{
	if (paused)
	{
		alSourcePause(source);
	}
	else
	{
		alSourcePlay(source);
	}
}

void Audio::Stop(ALuint source)
{
	alSourceStop(source);
}

bool Audio::GetLooping(ALuint source)
{
	ALint looping;
	alGetSourcei(source, AL_LOOPING, &looping);
	return (looping == AL_TRUE) ? true : false;
}

void Audio::SetLooping(ALuint source, bool looping)
{
	alSourcei(source, AL_LOOPING, looping);
}

void Audio::SetListenerTransform(ALfloat position[3], ALfloat velocity[3], ALfloat orientation[6])
{
	alListenerfv(AL_POSITION, position);
	alListenerfv(AL_VELOCITY, velocity);
	alListenerfv(AL_ORIENTATION, orientation);
}

void Audio::SetSourceTransform(ALuint source, ALfloat position[3], ALfloat velocity[3])
{
	alSourcefv(source, AL_POSITION, position);
	alSourcefv(source, AL_VELOCITY, velocity);
}

float Audio::GetSourceRefDist(ALuint source)
{
	float refDist;
	alGetSourcef(source, AL_REFERENCE_DISTANCE, &refDist);
	return refDist;
}

void Audio::SetSourceRefDist(ALuint source, float distance)
{
	alSourcef(source, AL_REFERENCE_DISTANCE, distance);
}

float Audio::GetSourceRolloff(ALuint source)
{
	float rolloffFactor;
	alGetSourcef(source, AL_ROLLOFF_FACTOR, &rolloffFactor);
	return rolloffFactor;
}

void Audio::SetSourceRolloff(ALuint source, float factor)
{
	alSourcef(source, AL_ROLLOFF_FACTOR, factor);
}

float Audio::GetSourceMaxDist(ALuint source)
{
	float maxDist;
	alGetSourcef(source, AL_MAX_DISTANCE, &maxDist);
	return maxDist;
}

void Audio::SetSourceMaxDist(ALuint source, float distance)
{
	alSourcef(source, AL_MAX_DISTANCE, distance);
}