#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include "logger.h"

// Tries to allocate the specified amount of bytes within a MAX_INT range from a module.
// If the module name is left unspecified, the main module is chosen by default.
// Big thanks to LukeYui for providing the code I based this on.
extern void* moduleAlloc(std::size_t cb, LPCSTR lpModuleName = NULL);