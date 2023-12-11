#include "alloc.h"

void* moduleAlloc(std::size_t cb, LPCSTR lpModuleName) {
	std::string moduleName;
	// Get the name of the base module if a module name wasn't already provided.
	if (lpModuleName == NULL) {
		auto moduleNameShort = std::make_unique<char[]>(MAX_PATH);
		moduleName = getCurrentModuleName<true>();
		if (errno_t error = _splitpath_s(moduleName.c_str(), NULL, 0, NULL, 0, moduleNameShort.get(), MAX_PATH, NULL, 0)) {
			Logger::log(Logger::ERR, "Failed to allocate memory: unable to get main module name; error %d", error);
			return nullptr;
		}
		else {
			moduleName = std::string(moduleNameShort.get()) + ".exe";
		}
	}
	else {
		moduleName = std::string(lpModuleName);
	}
	Logger::log(Logger::DEBUG, "Allocating %d bytes near module %s", cb, moduleName.c_str());
	MODULEINFO modInfo{};
	MEMORY_BASIC_INFORMATION memInfo{};
	if (HMODULE hModule = GetModuleHandleA(moduleName.c_str())) {
		if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO))) {
			Logger::log(Logger::ERR, "Failed to allocate memory: unable to get module information; error %d", GetLastError());
			return nullptr;
		}
	}
	else {
		Logger::log(Logger::ERR, "Failed to allocate memory: unable to get module handle; error %d", GetLastError());
		return nullptr;
	}
	uintptr_t moduleStart = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
	uintptr_t moduleEnd = moduleStart + modInfo.SizeOfImage;
	// Establish the base address to search for suitable memory from, it must be within an INT_MAX range from any point in the module.
	memInfo.BaseAddress = reinterpret_cast<void*>(moduleEnd + cb - INT_MAX);
	memInfo.RegionSize = 0;
	// Search for a free memory region of our size.
	while (VirtualQuery(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(memInfo.BaseAddress) + memInfo.RegionSize), &memInfo, sizeof(MEMORY_BASIC_INFORMATION))) {
		// Check if region is free and large enough.
		if (!(memInfo.State & MEM_FREE)) continue;
		if (cb > memInfo.RegionSize) continue;
		uintptr_t begin = reinterpret_cast<uintptr_t>(memInfo.BaseAddress);
		uintptr_t end = begin + memInfo.RegionSize;
		// Check if region is within INT_MAX bounds.
		if (end <= moduleStart && cb + moduleEnd - end > INT_MAX) continue;
		if (begin >= moduleEnd && cb + begin - moduleStart > INT_MAX) continue;
		// Found a suitable region.
		void* alloc = reinterpret_cast<void*>(end <= moduleStart ? end - cb : begin);
		alloc = VirtualAlloc(alloc, cb, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (alloc) {
			Logger::log(Logger::DEBUG, "Allocated %d bytes at %p", cb, alloc);
			return alloc;
		}
	}
	// Completing the loop means a suitable memory region was not found withing INT_MAX range of the module.
	Logger::log(Logger::ERR, "Failed to allocate memory, size %d: no suitable memory region found", cb);
	return nullptr;
}