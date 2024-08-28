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

#pragma once

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>

#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>

struct BufferData
{
	ALuint id;
	ALenum format;
};

class Audio
{
private:
	struct Request
	{
		ALuint source;
		std::string filePath;
	};

	static inline std::unordered_map<std::string, BufferData> buffers;
	static inline std::vector<ALuint> activeSources;
	static inline std::queue<Request> requestQueue;
	static inline std::mutex requestLock;
	
	static ALuint GetSource();

	static ALuint MakeBuffer(const std::string& filePath);

	static void SendSoundRequest(Request request);

public:
	static bool IsMono(ALuint source);
	static void CleanSources();
	static void CheckError(const std::string& where);
	static void ProcessSoundRequests();

	static ALuint PlaySoundEffect(const std::string& filePath);

	static float GetMainVolume();
	static void SetMainVolume(float gain);

	static float GetVolume(ALuint source);
	static void SetVolume(ALuint source, float gain);

	static bool GetPaused(ALuint source);
	static void SetPaused(ALuint source, bool paused);

	static void Stop(ALuint source);

	static bool GetLooping(ALuint source);
	static void SetLooping(ALuint source, bool looping);

	static void SetListenerTransform(ALfloat position[3], ALfloat velocity[3], ALfloat orientation[6]);
	static void SetSourceTransform(ALuint source, ALfloat position[3], ALfloat velocity[3]);

	static float GetSourceRefDist(ALuint source);
	static void SetSourceRefDist(ALuint source, float distance);

	static float GetSourceRolloff(ALuint source);
	static void SetSourceRolloff(ALuint source, float factor);

	static float GetSourceMaxDist(ALuint source);
	static void SetSourceMaxDist(ALuint source, float distance);

};