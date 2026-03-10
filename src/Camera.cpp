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

#include "Camera.h"

#include "LuaHelpers.h"
#include "Ogre.h"

#include <cmath>

namespace ExtraUtilities::Lua::Camera
{
	namespace
	{
		void* GetCurrentOgreCamera()
		{
			void* sceneManager = Ogre::sceneManager.Read();
			if (sceneManager == nullptr)
			{
				return nullptr;
			}

			__try
			{
				void* viewport = Ogre::GetCurrentViewport(sceneManager);
				if (viewport == nullptr)
				{
					return nullptr;
				}

				return Ogre::GetViewportCamera(viewport);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return nullptr;
			}
		}
	}

	int GetOrigins(lua_State* L)
	{
		BZR::BZR_Camera* cam = mainCam.Get();
		
		lua_createtable(L, 0, 4); // table with 4 non array (map) elements

		lua_pushnumber(L, cam->Orig_x);
		lua_setfield(L, -2, "Orig_x");

		lua_pushnumber(L, cam->Orig_y);
		lua_setfield(L, -2, "Orig_y");

		lua_pushnumber(L, cam->Const_x);
		lua_setfield(L, -2, "Const_x");

		lua_pushnumber(L, cam->Const_y);
		lua_setfield(L, -2, "Const_y");

		return 1;
	}

	int GetTransformMatrix(lua_State* L)
	{
		BZR::MAT_3D viewMatrix = mainCam.Get()->Matrix;
		
		BZR::MAT_3D transformMatrix;
		BZR::Matrix_Inverse(&transformMatrix, &viewMatrix);
		
		PushMatrix(L, transformMatrix);

		return 1;
	}

	int GetViewMatrix(lua_State* L)
	{
		BZR::MAT_3D viewMatrix = mainCam.Get()->Matrix;

		PushMatrix(L, viewMatrix);

		return 1;
	}

	int GetMaxZoom(lua_State* L)
	{
		lua_pushnumber(L, maxZoom.Read());
		return 1;
	}

	int SetMaxZoom(lua_State* L)
	{
		float zoom = static_cast<float>(luaL_checknumber(L, 1));
		maxZoom.Write(zoom);
		return 0;
	}

	int GetMinZoom(lua_State* L)
	{
		lua_pushnumber(L, minZoom.Read());
		return 1;
	}

	int SetMinZoom(lua_State* L)
	{
		float zoom = static_cast<float>(luaL_checknumber(L, 1));
		minZoom.Write(zoom);
		return 0;
	}

	int GetView(lua_State* L)
	{
		lua_pushinteger(L, currentView.Read());
		return 1;
	}

	int GetFOV(lua_State* L)
	{
		auto* cam = mainCam.Get();
		lua_pushnumber(L, cam ? cam->View_Angle : 0.0f);
		return 1;
	}

	int GetClipDistances(lua_State* L)
	{
		void* camera = GetCurrentOgreCamera();
		if (camera == nullptr)
		{
			lua_pushnil(L);
			lua_pushnil(L);
			return 2;
		}

		__try
		{
			lua_pushnumber(L, Ogre::GetCameraNearClipDistance(camera));
			lua_pushnumber(L, Ogre::GetCameraFarClipDistance(camera));
			return 2;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			lua_pushnil(L);
			lua_pushnil(L);
			return 2;
		}
	}

	int SetClipDistances(lua_State* L)
	{
		float nearClip = static_cast<float>(luaL_checknumber(L, 1));
		float farClip = static_cast<float>(luaL_checknumber(L, 2));
		if (!std::isfinite(nearClip) || nearClip < 0.0f)
		{
			return luaL_argerror(L, 1, "near clip distance must be a finite non-negative number");
		}
		if (!std::isfinite(farClip) || farClip < 0.0f)
		{
			return luaL_argerror(L, 2, "far clip distance must be a finite non-negative number");
		}

		void* camera = GetCurrentOgreCamera();
		if (camera == nullptr)
		{
			return 0;
		}

		__try
		{
			Ogre::SetFrustumNearClipDistance(camera, nearClip);
			Ogre::SetFrustumFarClipDistance(camera, farClip);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

		return 0;
	}

	int GetAspectRatio(lua_State* L)
	{
		void* camera = GetCurrentOgreCamera();
		if (camera == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		__try
		{
			lua_pushnumber(L, Ogre::GetFrustumAspectRatio(camera));
			return 1;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			lua_pushnil(L);
			return 1;
		}
	}

	int SetAspectRatio(lua_State* L)
	{
		float ratio = static_cast<float>(luaL_checknumber(L, 1));
		if (!std::isfinite(ratio) || ratio <= 0.0f)
		{
			return luaL_argerror(L, 1, "aspect ratio must be a finite number greater than zero");
		}

		void* camera = GetCurrentOgreCamera();
		if (camera == nullptr)
		{
			return 0;
		}

		__try
		{
			Ogre::SetFrustumAspectRatio(camera, ratio);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

		return 0;
	}

	int GetProjectionType(lua_State* L)
	{
		void* camera = GetCurrentOgreCamera();
		if (camera == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		__try
		{
			lua_pushinteger(L, Ogre::GetFrustumProjectionType(camera));
			return 1;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			lua_pushnil(L);
			return 1;
		}
	}

	int SetProjectionType(lua_State* L)
	{
		int projectionType = luaL_checkinteger(L, 1);
		if (projectionType < 0 || projectionType > 1)
		{
			return luaL_argerror(L, 1, "projection type must be 0 (orthographic) or 1 (perspective)");
		}

		void* camera = GetCurrentOgreCamera();
		if (camera == nullptr)
		{
			return 0;
		}

		__try
		{
			Ogre::SetFrustumProjectionType(camera, projectionType);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

		return 0;
	}

	int GetPolygonMode(lua_State* L)
	{
		void* camera = GetCurrentOgreCamera();
		if (camera == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		__try
		{
			lua_pushinteger(L, Ogre::GetCameraPolygonMode(camera));
			return 1;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			lua_pushnil(L);
			return 1;
		}
	}

	int SetPolygonMode(lua_State* L)
	{
		int polygonMode = luaL_checkinteger(L, 1);
		if (polygonMode < 1 || polygonMode > 3)
		{
			return luaL_argerror(L, 1, "polygon mode must be 1 (points), 2 (wireframe), or 3 (solid)");
		}

		void* camera = GetCurrentOgreCamera();
		if (camera == nullptr)
		{
			return 0;
		}

		__try
		{
			Ogre::SetCameraPolygonMode(camera, polygonMode);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

		return 0;
	}

	int SetView(lua_State* L)
	{
		int view = luaL_checkinteger(L, 1);

		using enum BZR::Camera::View;
		switch (view)
		{
		case COCKPIT:
			break;
		case NO_COCKPIT:
			break;
		case CHASE:
			break;
		case ORBIT:
			break;
		case NO_HUD:
			break;
		case FREECAM:
			break;
		default:
			luaL_argerror(L, 1, "Extra Utilities Error: Invalid view mode");
			return 0;
		}

		BZR::Camera::Set_View(userEntity.Read(), view);

		return 0;
	}

	int GetZoom(lua_State* L)
	{
		int camera = luaL_checkinteger(L, 1);

		using enum BZR::Camera::View;
		switch (camera)
		{
		case FIRST_PERSON:
			lua_pushnumber(L, zoomFPP.Read());
			break;
		case THIRD_PERSON:
			lua_pushnumber(L, zoomTPP.Read());
			break;
		default:
			luaL_argerror(L, 1, "Extra Utilities Error: Invalid camera");
			return 0;
		}
		return 1;
	}

	int SetZoom(lua_State* L)
	{
		int camera = luaL_checkinteger(L, 1);
		float zoom = static_cast<float>(luaL_checknumber(L, 2));

		using enum BZR::Camera::View;
		switch (camera)
		{
		case FIRST_PERSON:
			zoomFPP.Write(zoom);
			break;
		case THIRD_PERSON:
			zoomTPP.Write(zoom);
			break;
		default:
			luaL_argerror(L, 1, "Extra Utilities Error: Invalid camera");
			return 0;
		}
		return 0;
	}
}
