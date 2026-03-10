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

#include "Overlay.h"

#include "LuaHelpers.h"
#include "OgreOverlayShim.h"

#include <Windows.h>

#include <cmath>
#include <string>
#include <unordered_map>

namespace ExtraUtilities::Lua::Overlay
{
	namespace
	{
		enum class ElementKind
		{
			Unknown,
			Panel,
			BorderPanel,
		};

		std::unordered_map<std::string, ElementKind> knownElements;

		ElementKind GetElementKindByTypeName(const std::string& typeName)
		{
			if (typeName == "Panel")
			{
				return ElementKind::Panel;
			}
			if (typeName == "BorderPanel")
			{
				return ElementKind::BorderPanel;
			}
			return ElementKind::Unknown;
		}

		bool IsContainerKind(ElementKind kind)
		{
			return kind == ElementKind::Panel || kind == ElementKind::BorderPanel;
		}

		::Ogre::OverlayManager* GetOverlayManager()
		{
			__try
			{
				return ::Ogre::OverlayManager::getSingletonPtr();
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return nullptr;
			}
		}

		::Ogre::Overlay* FindOverlay(const std::string& name)
		{
			::Ogre::OverlayManager* manager = GetOverlayManager();
			if (manager == nullptr)
			{
				return nullptr;
			}

			__try
			{
				return manager->getByName(name);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return nullptr;
			}
		}

		::Ogre::OverlayElement* FindOverlayElement(const std::string& name)
		{
			::Ogre::OverlayManager* manager = GetOverlayManager();
			if (manager == nullptr)
			{
				return nullptr;
			}

			__try
			{
				if (!manager->hasOverlayElement(name, false))
				{
					return nullptr;
				}
				return manager->getOverlayElement(name, false);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return nullptr;
			}
		}

		::Ogre::OverlayContainer* FindOverlayContainer(const std::string& name)
		{
			const auto it = knownElements.find(name);
			if (it == knownElements.end() || !IsContainerKind(it->second))
			{
				return nullptr;
			}

			return reinterpret_cast<::Ogre::OverlayContainer*>(FindOverlayElement(name));
		}

		bool IsFiniteColor(const ExtraUtilities::Ogre::Color& color)
		{
			return std::isfinite(color.r)
				&& std::isfinite(color.g)
				&& std::isfinite(color.b)
				&& std::isfinite(color.a);
		}

		bool SetOverlayParameter(::Ogre::OverlayElement* element, const std::string& name, const std::string& value)
		{
			using SetParameterFn = bool(__thiscall*)(void*, const std::string&, const std::string&);
			static SetParameterFn setParameter = []()
			{
				HMODULE ogreMain = GetModuleHandleA("OgreMain.dll");
				if (ogreMain == nullptr)
				{
					return static_cast<SetParameterFn>(nullptr);
				}

				return reinterpret_cast<SetParameterFn>(
					GetProcAddress(ogreMain, "?setParameter@StringInterface@Ogre@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@0@Z"));
			}();

			if (setParameter == nullptr || element == nullptr)
			{
				return false;
			}

			return setParameter(element, name, value);
		}
	}

	int CreateOverlay(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		::Ogre::OverlayManager* manager = GetOverlayManager();
		if (manager == nullptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		if (manager->getByName(name) != nullptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		lua_pushboolean(L, manager->create(name) != nullptr);
		return 1;
	}

	int DestroyOverlay(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		::Ogre::OverlayManager* manager = GetOverlayManager();
		if (manager == nullptr)
		{
			return 0;
		}

		if (manager->getByName(name) != nullptr)
		{
			manager->destroy(name);
		}

		return 0;
	}

	int ShowOverlay(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		::Ogre::Overlay* overlay = FindOverlay(name);
		if (overlay == nullptr)
		{
			return 0;
		}

		overlay->show();
		return 0;
	}

	int HideOverlay(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		::Ogre::Overlay* overlay = FindOverlay(name);
		if (overlay == nullptr)
		{
			return 0;
		}

		overlay->hide();
		return 0;
	}

	int SetOverlayZOrder(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const int zOrder = static_cast<int>(luaL_checkinteger(L, 2));
		if (zOrder < 0 || zOrder > 650)
		{
			return luaL_argerror(L, 2, "z-order must be between 0 and 650");
		}

		::Ogre::Overlay* overlay = FindOverlay(name);
		if (overlay == nullptr)
		{
			return 0;
		}

		overlay->setZOrder(static_cast<unsigned short>(zOrder));
		return 0;
	}

	int SetOverlayScroll(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const float x = static_cast<float>(luaL_checknumber(L, 2));
		const float y = static_cast<float>(luaL_checknumber(L, 3));
		if (!std::isfinite(x))
		{
			return luaL_argerror(L, 2, "scroll x must be finite");
		}
		if (!std::isfinite(y))
		{
			return luaL_argerror(L, 3, "scroll y must be finite");
		}

		::Ogre::Overlay* overlay = FindOverlay(name);
		if (overlay == nullptr)
		{
			return 0;
		}

		overlay->setScroll(x, y);
		return 0;
	}

	int CreateOverlayElement(lua_State* L)
	{
		const std::string typeName = luaL_checkstring(L, 1);
		const std::string instanceName = luaL_checkstring(L, 2);
		::Ogre::OverlayManager* manager = GetOverlayManager();
		if (manager == nullptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		if (manager->hasOverlayElement(instanceName, false))
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		::Ogre::OverlayElement* element = manager->createOverlayElement(typeName, instanceName, false);
		if (element != nullptr)
		{
			knownElements[instanceName] = GetElementKindByTypeName(typeName);
		}

		lua_pushboolean(L, element != nullptr);
		return 1;
	}

	int DestroyOverlayElement(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		::Ogre::OverlayManager* manager = GetOverlayManager();
		if (manager == nullptr)
		{
			return 0;
		}

		if (manager->hasOverlayElement(name, false))
		{
			manager->destroyOverlayElement(name, false);
		}
		knownElements.erase(name);

		return 0;
	}

	int HasOverlayElement(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		::Ogre::OverlayManager* manager = GetOverlayManager();
		if (manager == nullptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		lua_pushboolean(L, manager->hasOverlayElement(name, false));
		return 1;
	}

	int AddOverlay2D(lua_State* L)
	{
		const std::string overlayName = luaL_checkstring(L, 1);
		const std::string containerName = luaL_checkstring(L, 2);

		::Ogre::Overlay* overlay = FindOverlay(overlayName);
		::Ogre::OverlayContainer* container = FindOverlayContainer(containerName);
		if (overlay == nullptr || container == nullptr)
		{
			return 0;
		}

		overlay->add2D(container);
		return 0;
	}

	int RemoveOverlay2D(lua_State* L)
	{
		const std::string overlayName = luaL_checkstring(L, 1);
		const std::string containerName = luaL_checkstring(L, 2);

		::Ogre::Overlay* overlay = FindOverlay(overlayName);
		::Ogre::OverlayContainer* container = FindOverlayContainer(containerName);
		if (overlay == nullptr || container == nullptr)
		{
			return 0;
		}

		overlay->remove2D(container);
		return 0;
	}

	int AddOverlayElementChild(lua_State* L)
	{
		const std::string parentName = luaL_checkstring(L, 1);
		const std::string childName = luaL_checkstring(L, 2);

		::Ogre::OverlayContainer* parent = FindOverlayContainer(parentName);
		::Ogre::OverlayElement* child = FindOverlayElement(childName);
		if (parent == nullptr || child == nullptr)
		{
			return 0;
		}

		parent->::Ogre::OverlayContainer::addChild(child);
		return 0;
	}

	int RemoveOverlayElementChild(lua_State* L)
	{
		const std::string parentName = luaL_checkstring(L, 1);
		const std::string childName = luaL_checkstring(L, 2);

		::Ogre::OverlayContainer* parent = FindOverlayContainer(parentName);
		if (parent == nullptr)
		{
			return 0;
		}

		parent->::Ogre::OverlayContainer::removeChild(childName);
		return 0;
	}

	int ShowOverlayElement(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		element->::Ogre::OverlayElement::show();
		return 0;
	}

	int HideOverlayElement(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		element->::Ogre::OverlayElement::hide();
		return 0;
	}

	int SetOverlayMetricsMode(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const int mode = static_cast<int>(luaL_checkinteger(L, 2));
		if (mode < static_cast<int>(::Ogre::GMM_RELATIVE) || mode > static_cast<int>(::Ogre::GMM_RELATIVE_ASPECT_ADJUSTED))
		{
			return luaL_argerror(L, 2, "metrics mode must be a valid exu.OVERLAY_METRICS value");
		}

		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		element->::Ogre::OverlayElement::setMetricsMode(static_cast<::Ogre::GuiMetricsMode>(mode));
		return 0;
	}

	int SetOverlayPosition(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const float left = static_cast<float>(luaL_checknumber(L, 2));
		const float top = static_cast<float>(luaL_checknumber(L, 3));
		if (!std::isfinite(left))
		{
			return luaL_argerror(L, 2, "left must be finite");
		}
		if (!std::isfinite(top))
		{
			return luaL_argerror(L, 3, "top must be finite");
		}

		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		element->setPosition(left, top);
		return 0;
	}

	int SetOverlayDimensions(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const float width = static_cast<float>(luaL_checknumber(L, 2));
		const float height = static_cast<float>(luaL_checknumber(L, 3));
		if (!std::isfinite(width))
		{
			return luaL_argerror(L, 2, "width must be finite");
		}
		if (!std::isfinite(height))
		{
			return luaL_argerror(L, 3, "height must be finite");
		}

		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		element->setDimensions(width, height);
		return 0;
	}

	int SetOverlayMaterial(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const std::string materialName = luaL_checkstring(L, 2);

		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		element->::Ogre::OverlayElement::setMaterialName(materialName);
		return 0;
	}

	int SetOverlayColor(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const ExtraUtilities::Ogre::Color color = CheckColorOrSingles(L, 2);
		if (!IsFiniteColor(color))
		{
			return luaL_argerror(L, 2, "color components must be finite");
		}

		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		const ::Ogre::ColourValue ogreColor{ color.r, color.g, color.b, color.a };
		element->::Ogre::OverlayElement::setColour(ogreColor);
		return 0;
	}

	int SetOverlayCaption(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const std::string text = luaL_checkstring(L, 2);

		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		SetOverlayParameter(element, "caption", text);
		return 0;
	}

	int SetOverlayTextFont(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const std::string fontName = luaL_checkstring(L, 2);

		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		SetOverlayParameter(element, "font_name", fontName);
		return 0;
	}

	int SetOverlayTextCharHeight(lua_State* L)
	{
		const std::string name = luaL_checkstring(L, 1);
		const float charHeight = static_cast<float>(luaL_checknumber(L, 2));
		if (!std::isfinite(charHeight))
		{
			return luaL_argerror(L, 2, "char height must be finite");
		}

		::Ogre::OverlayElement* element = FindOverlayElement(name);
		if (element == nullptr)
		{
			return 0;
		}

		SetOverlayParameter(element, "char_height", std::to_string(charHeight));
		return 0;
	}
}
