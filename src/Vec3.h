/* Copyright (C) 2023-2025 VTrider
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

#include <cmath>

namespace ExtraUtilities
{
	class Vec3
	{
	public:
		float x, y, z;

		Vec3() : x(0.0f), y(0.0f), z(0.0f) {};
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
		Vec3(double x, double y, double z) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z)) {}

		float Length() const
		{
			return std::sqrt(std::powf(x, 2) + std::powf(y, 2) + std::powf(z, 2));
		}

		Vec3& Normalize()
		{
			float len = Length();
			x /= len;
			y /= len;
			z /= len;
			return *this;
		}

		friend Vec3 operator+(const Vec3& v, const Vec3& w);
		friend Vec3 operator-(const Vec3& v, const Vec3& w);
		friend Vec3 operator*(const Vec3& v, const Vec3& w);
		friend Vec3 operator*(const Vec3& v, float s);
	};

	inline Vec3 operator+(const Vec3& v, const Vec3& w)
	{
		return Vec3(v.x + w.x, v.y + w.y, v.z + w.z);
	}

	inline Vec3 operator-(const Vec3& v, const Vec3& w)
	{
		return Vec3(v.x - w.x, v.y - w.y, v.z - w.z);
	}

	inline Vec3 operator*(const Vec3& v, const Vec3& w)
	{
		return Vec3(v.x * w.x, v.y * w.y, v.z * w.z);
	}

	inline Vec3 operator*(const Vec3& v, float s)
	{
		return Vec3(v.x * s, v.y * s, v.z * s);
	}
}