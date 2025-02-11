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

#pragma once

#include <lua.hpp>
#include <Windows.h>

#include <algorithm>
#include <string>
#include <unordered_map>

// Debug mode doesn't like the C toupper function, but it's fine
#ifdef _DEBUG
#pragma warning(disable: 4244)
#endif

namespace ExtraUtilities::Lua::IO
{
	// Convert string keys to windows vKeys for use in GetAsyncKeyState
	inline const std::unordered_map<std::string, int> keyMap = {
	{"0", 0x30}, {"1", 0x31}, {"2", 0x32}, {"3", 0x33}, {"4", 0x34},
	{"5", 0x35}, {"6", 0x36}, {"7", 0x37}, {"8", 0x38}, {"9", 0x39},
	{"A", 0x41}, {"B", 0x42}, {"C", 0x43}, {"D", 0x44}, {"E", 0x45},
	{"F", 0x46}, {"G", 0x47}, {"H", 0x48}, {"I", 0x49}, {"J", 0x4A},
	{"K", 0x4B}, {"L", 0x4C}, {"M", 0x4D}, {"N", 0x4E}, {"O", 0x4F},
	{"P", 0x50}, {"Q", 0x51}, {"R", 0x52}, {"S", 0x53}, {"T", 0x54},
	{"U", 0x55}, {"V", 0x56}, {"W", 0x57}, {"X", 0x58}, {"Y", 0x59},
	{"Z", 0x5A}, {"CTRL", VK_CONTROL}, {"LCTRL", VK_LCONTROL}, {"RCTRL", VK_RCONTROL},
	{"ALT", VK_MENU}, {"LALT", VK_LMENU}, {"RALT", VK_RMENU}, {"SHIFT", VK_SHIFT},
	{"LSHIFT", VK_LSHIFT}, {"RSHIFT", VK_RSHIFT}, {"CAPSLOCK", VK_CAPITAL},
	{"TAB", VK_TAB}, {"PAUSE", VK_PAUSE}, {"PAGEUP", VK_PRIOR}, {"PAGEDOWN", VK_NEXT},
	{"END", VK_END}, {"HOME", VK_HOME}, {"INSERT", VK_INSERT}, {"DELETE", VK_DELETE},
	{"BACKSPACE", VK_BACK}, {"ENTER", VK_RETURN}, {"SPACE", VK_SPACE},
	{"LMB", VK_LBUTTON}, {"RMB", VK_RBUTTON}, {"MMB", VK_MBUTTON}, {"XB1", VK_XBUTTON1},
	{"XB2", VK_XBUTTON2}, {"ESCAPE", VK_ESCAPE}, {"LARROW", VK_LEFT}, {"UARROW", VK_UP},
	{"RARROW", VK_RIGHT}, {"DARROW", VK_DOWN}, {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3},
	{"F4", VK_F4}, {"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8},
	{"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
	};

	inline std::string ToUpper(const std::string& str)
	{
		std::string newStr = str;
		std::transform(newStr.begin(), newStr.end(), newStr.begin(), ::toupper);
		return newStr;
	}

	int GetGameKey(lua_State* L);
}