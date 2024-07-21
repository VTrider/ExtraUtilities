#pragma once

#include "bzr.h"

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