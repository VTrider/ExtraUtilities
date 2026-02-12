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

#include "ControlPanel.h"

#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::ControlPanel
{
	int SelectAdd(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::GameObject* obj = BZR::GameObject::GetObj(h);
		BZR::ControlPanel::SelectAdd(controlPanel, obj);
		return 0;
	}

	int SelectNone(lua_State*)
	{
		BZR::ControlPanel::SelectNone(controlPanel);
		return 0;
	}

	int SelectOne(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::GameObject* obj = BZR::GameObject::GetObj(h);
		BZR::ControlPanel::SelectOne(controlPanel, obj);
		return 0;
	}
}