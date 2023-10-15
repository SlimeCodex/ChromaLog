/*
* This file is part of ChromaLog Library.
* Copyright (C) 2023 Alejandro Nicolini
* 
* ChromaLog is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* ChromaLog is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with ChromaLog. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ___CHROMA_LOG_H___
#define ___CHROMA_LOG_H___

#include <cstdarg>
#include <string>
#include <cstdio>
#include <regex>
#include <map>

// LOG Helpers
#define ANSI_RESET	"\033[0m"	// Reset color
#define ANSI_CYAN	"\033[36m"	// For timestamp

class ChromaLog {
private:
	static std::map<std::string, std::string> colorMap;

	static std::string process(const std::string& message) {
#ifndef CHROMA_LOG_COLOR_ENABLED
		std::regex pattern(R"(~(\w+)\(([^)]+)\)~)");
		return std::regex_replace(message, pattern, "$2");  // Strip out color tags, keep format specifiers
#else
		std::regex pattern(R"(~(\w+)\(([^)]+)\)~)");
		std::smatch matches;
		std::string processed = message;

		while (std::regex_search(processed, matches, pattern)) {
			std::string replacement;
			if (colorMap.find(matches[1]) != colorMap.end()) {
				replacement = colorMap[matches[1].str()] + matches[2].str() + ANSI_RESET;
			} else {
				replacement = matches[2];  // if color not recognized, just use the format specifier
			}
			processed = std::regex_replace(processed, pattern, replacement, std::regex_constants::format_first_only);
		}
		return processed;
#endif
	}

public:
	static void log(const char* file, int line, const char* func, const char* format, ...) {
		std::string processedFormat = process(std::string(format));

#ifdef CHROMA_LOG_OUTPUT_ADVANCED
		printf("[%s:%d] %s(): ", file, line, func);
#endif
		va_list args;
		va_start(args, format);
		vprintf(processedFormat.c_str(), args);
		va_end(args);
	}
};

#define CHROMA_LOG(fmt, ...) ChromaLog::log(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#endif // ___CHROMA_LOG_H___
