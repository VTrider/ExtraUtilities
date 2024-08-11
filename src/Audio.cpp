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
		return 0;
	}

	alBufferData(bufferID, format, samples.data(), samples.size() * sizeof(short), fileInfo.samplerate);

	buffers.emplace(filePath, bufferID);
	return bufferID;
}

ALuint Audio::MakeSource()
{
	ALuint sourceID;
	alGenSources(1, &sourceID);
	requestLock.lock();
	sources.push_back(sourceID);
	requestLock.unlock();
	return sourceID;
}

void Audio::SendSoundRequest(ALuint source)
{
	requestLock.lock();
	requestQueue.push(source);
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

	ALuint sourceToPlay = requestQueue.front();
	requestQueue.pop();
	requestLock.unlock();

	alSourcePlay(sourceToPlay);
}

void Audio::CleanSources()
{
	requestLock.lock();
	for (const auto& source : sources)
	{
		ALint playing;
		alGetSourcei(source, AL_SOURCE_STATE, &playing);
		if (playing != AL_PLAYING)
		{
			alDeleteSources(1, &source);
		}
	}
	requestLock.unlock();
}

ALuint Audio::PlaySoundEffect(const std::string& filePath)
{

	ALuint buffer;

	// If the buffer already exists ie. you already loaded that sound file
	// into memory then use it, otherwise make a new one
	auto it = buffers.find(filePath);
	if (it != buffers.end())
	{
		buffer = it->second;
	}
	else
	{
		buffer = MakeBuffer(filePath);
	}

	ALuint source = MakeSource();

	alSourcei(source, AL_BUFFER, buffer);
	SendSoundRequest(source);
	return source;
}