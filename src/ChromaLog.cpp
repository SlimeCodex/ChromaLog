/*
* This file is part of SmartSyncEvent Library.
* Copyright (C) 2023 Alejandro Nicolini
* 
* SmartSyncEvent is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* SmartSyncEvent is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with SmartSyncEvent. If not, see <https://www.gnu.org/licenses/>.
*/

#include <ChromaLog.h>

std::map<std::string, std::string> ChromaLog::colorMap = {
	{"black", "\033[30m"},
	{"red", "\033[31m"},
	{"green", "\033[32m"},
	{"yellow", "\033[33m"},
	{"blue", "\033[34m"},
	{"magenta", "\033[35m"},
	{"cyan", "\033[36m"},
	{"white", "\033[37m"},

	{"b_black", "\033[90m"},
	{"b_red", "\033[91m"},
	{"b_green", "\033[92m"},
	{"b_yellow", "\033[93m"},
	{"b_blue", "\033[94m"},
	{"b_magenta", "\033[95m"},
	{"b_cyan", "\033[96m"},
	{"b_white", "\033[97m"},

	{"bg_black", "\033[40m"},
	{"bg_red", "\033[41m"},
	{"bg_green", "\033[42m"},
	{"bg_yellow", "\033[43m"},
	{"bg_blue", "\033[44m"},
	{"bg_magenta", "\033[45m"},
	{"bg_cyan", "\033[46m"},
	{"bg_white", "\033[47m"},

	{"bg_b_black", "\033[100m"},
	{"bg_b_red", "\033[101m"},
	{"bg_b_green", "\033[102m"},
	{"bg_b_yellow", "\033[103m"},
	{"bg_b_blue", "\033[104m"},
	{"bg_b_magenta", "\033[105m"},
	{"bg_b_cyan", "\033[106m"},
	{"bg_b_white", "\033[107m"}
};