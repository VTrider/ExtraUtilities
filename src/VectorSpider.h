/* Copyright (C) 2023-2026 VTrider
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

#include <vector>

namespace ExtraUtilities
{
	// Crawls an std::vector in memory from the given address
	template <typename T>
	std::vector<T> VectorSpider(const void* address)
	{
		std::vector<T> results;

		T*** p = (T***)address;
		T* begin = **p;
		T* end = *(*p + 1);

		while (begin != end)
		{
			results.push_back(reinterpret_cast<T>(*begin));
			begin++;
		}
		
		return results;
	}
}