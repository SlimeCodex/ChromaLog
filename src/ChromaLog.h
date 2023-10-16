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

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include <string>
#include <vector>
#include <map>

#include <Arduino.h>
#define CHROMA_LOG_CHANNEL	Serial

// LOG Helpers
#define ANSI_RESET		"\033[0m"
#define ANSI_BLACK		"\033[90m"
#define ANSI_RED		"\033[91m"
#define ANSI_GREEN		"\033[92m"
#define ANSI_YELLOW		"\033[93m"
#define ANSI_BLUE		"\033[94m"
#define ANSI_MAGENTA	"\033[95m"
#define ANSI_CYAN		"\033[96m"
#define ANSI_WHITE		"\033[97m"
#define ANSI_BG_BLACK	"\033[100m"
#define ANSI_BG_RED		"\033[101m"
#define ANSI_BG_GREEN	"\033[102m"
#define ANSI_BG_YELLOW	"\033[103m"
#define ANSI_BG_BLUE	"\033[104m"
#define ANSI_BG_MAGENTA	"\033[105m"
#define ANSI_BG_CYAN	"\033[106m"
#define ANSI_BG_WHITE	"\033[107m"

#define CHROMA_LOG_TIMESTAMP_COLOR	ANSI_BG_BLACK
#define CHROMA_LOG_FILE_LINE_COLOR	ANSI_BG_BLACK
#define CHROMA_LOG_FUNC_COLOR		ANSI_BG_BLACK

class ChromaLog {
private:
	static SemaphoreHandle_t log_mutex;
	static std::map<std::string, std::string> colorMap;

	static std::string process(const std::string& message) {
#ifndef CHROMA_LOG_COLOR
		// Find all patterns and simply remove them
		std::string processed = message;
		size_t start = 0;
		while ((start = processed.find('~', start)) != std::string::npos) {
			size_t openPar = processed.find('(', start);
			size_t closePar = processed.find(')', openPar);
			if (openPar != std::string::npos && closePar != std::string::npos) {
				processed = processed.substr(0, start) + processed.substr(openPar + 1, closePar - openPar - 1) + processed.substr(closePar + 2);
			}
			start = start + 1; // move past the current '~'
		}
		return processed;
#else
		std::string processed = message;
		size_t start = 0;
		while ((start = processed.find('~', start)) != std::string::npos) {
			size_t openPar = processed.find('(', start);
			size_t closePar = processed.find(')', openPar);
			if (openPar != std::string::npos && closePar != std::string::npos) {
				std::string color = processed.substr(start + 1, openPar - start - 1);
				std::string content = processed.substr(openPar + 1, closePar - openPar - 1);

				std::string replacement;
				if (colorMap.find(color) != colorMap.end()) {
					replacement = colorMap[color] + content + ANSI_RESET;
				} else {
					replacement = content;  // if color not recognized, just use the format specifier
				}

				processed = processed.substr(0, start) + replacement + processed.substr(closePar + 2);
				start = start + replacement.length();
			} else {
				start = start + 1; // move past the current '~'
			}
		}
		return processed;
#endif
	}
public:
	static void log(const char* file, int line, const char* func, const char* format, ...) {
		if (xSemaphoreTake(log_mutex, portMAX_DELAY) == pdTRUE) {

			// Timestamp injection using FreeRTOS's xTaskGetTickCount
			#ifdef CHROMA_LOG_TIMESTAMP
			CHROMA_LOG_CHANNEL.printf(CHROMA_LOG_TIMESTAMP_COLOR " %" PRIu32 " " ANSI_RESET, xTaskGetTickCount());
			#endif

			// File and Line injection
			#ifdef CHROMA_LOG_FILE_LINE
			CHROMA_LOG_CHANNEL.printf(" ");
			CHROMA_LOG_CHANNEL.printf(CHROMA_LOG_FILE_LINE_COLOR " %s:%d " ANSI_RESET, file, line);
			#endif

			// Function injection
			#ifdef CHROMA_LOG_FUNC
			CHROMA_LOG_CHANNEL.printf(" ");
			CHROMA_LOG_CHANNEL.printf(CHROMA_LOG_FUNC_COLOR " %s " ANSI_RESET, func);
			#endif

			CHROMA_LOG_CHANNEL.printf(" ");

			va_list args;
			va_start(args, format);

			int requiredSize = std::vsnprintf(nullptr, 0, format, args);
			va_end(args);

			if (requiredSize <= 0) return;

			std::vector<char> buffer(requiredSize + 1);
			va_start(args, format);
			std::vsnprintf(buffer.data(), buffer.size(), format, args);
			va_end(args);

			std::string formattedMessage = buffer.data();
			std::string processed = process(formattedMessage);

			CHROMA_LOG_CHANNEL.printf("%s", processed.c_str());
			xSemaphoreGive(log_mutex);
		}
	}
};

#define CHROMA_LOG(fmt, ...) ChromaLog::log(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#endif // ___CHROMA_LOG_H___