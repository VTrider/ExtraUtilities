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

class Audio
{
private:
	static inline std::unordered_map<std::string, ALuint> buffers;
	static inline std::vector<ALuint> sources;
	static inline std::queue<ALuint> requestQueue;
	static inline std::mutex requestLock;

	static ALuint MakeBuffer(const std::string& filePath);
	static ALuint MakeSource();

	static void SendSoundRequest(ALuint source);

	

public:
	static void ProcessSoundRequests();
	static void CleanSources();

	static ALuint PlaySoundEffect(const std::string& filePath);

};