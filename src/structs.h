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

#include <bzr.h>

// This file is for newly found structs for BZR, to supplement janne's bzr.h file

struct Frustum
{
	VECTOR_3D near_top_left;
	VECTOR_3D near_top_right;
	VECTOR_3D near_bottom_right;
	VECTOR_3D near_bottom_left;
	VECTOR_3D far_top_left;
	VECTOR_3D far_top_right;
	VECTOR_3D far_bottom_right;
	VECTOR_3D far_bottom_left;
};

struct VECTOR_3D_DOUBLE
{
	double x;
	double y;
	double z;
};