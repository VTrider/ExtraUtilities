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
		SystemLog->Out("Failed to open sound file", 1);
		return 0;
	}

	std::vector<short> samples(static_cast<size_t>(fileInfo.frames) * static_cast<size_t>(fileInfo.channels));
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
		return 0;
	}

	alBufferData(bufferID, format, samples.data(), samples.size() * sizeof(short), fileInfo.samplerate);

	buffers.emplace(filePath, bufferID);
	return bufferID;
}

void Audio::SendSoundRequest(Request request)
{
	requestLock.lock();
	requestQueue.push(request);
	requestLock.unlock();
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

void Audio::CheckError(const std::string& where)
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR) {
		SystemLog->Out(std::format("Extra Utilities Error in: {}", where));
		switch (error) {
		case AL_INVALID_NAME:
			SystemLog->Out("OpenAL Error: Invalid Name", 1);
			break;
		case AL_INVALID_ENUM:
			SystemLog->Out("OpenAL Error: Invalid Enum", 1);
			break;
		case AL_INVALID_VALUE:
			SystemLog->Out("OpenAL Error: Invalid Value", 1);
			break;
		case AL_INVALID_OPERATION:
			SystemLog->Out("OpenAL Error: Invalid Operation", 1);
			break;
		case AL_OUT_OF_MEMORY:
			SystemLog->Out("OpenAL Error: Out Of Memory", 1);
			break;
		default:
			SystemLog->Out("OpenAL Error: Unknown Error", 1);
			break;
		}
	}
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
		buffer = it->second;
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