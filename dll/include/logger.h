#pragma once

#include "Windows.h"

#include <mutex>
#include <string>
#include <cstdarg>
#include <ctime>

namespace Logger {
	enum Level {
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERR,
		FATAL
	};

	extern void log(Level level, std::string message, ...);
	extern void log(const std::string& log);
	extern bool allocateConsole();
}

extern HINSTANCE getCurrentModule();

// Retrieves the full path of the current module or the current main module.
template <bool getMainModule = false> std::string getCurrentModuleName() {
	auto modulePath = std::make_unique<char[]>(MAX_PATH);
	// GetModuleHandleExA returns the handle to the current module from an address within its memory.
	// If the handle itself is NULL, the call to GetModuleFileName will return the filename of the main module.
	HINSTANCE hinstDLL;
	if constexpr (getMainModule) hinstDLL = NULL;
	else hinstDLL = getCurrentModule();
	// Get and return the full module path, or an empty string on failure.
	if (!GetModuleFileNameA(hinstDLL, modulePath.get(), MAX_PATH)) {
		return "";
	}
	return std::string(modulePath.get());
}