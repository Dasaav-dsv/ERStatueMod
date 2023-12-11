#pragma once

#include <cstdint>

class CSChrIns;

class hkHkbBehGraph {
public:
	bool& getUpdateState() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x1B2); }
};

class hkHkbCharacter {
public:
	CSChrIns* getChrIns() { return *reinterpret_cast<CSChrIns**>(reinterpret_cast<uintptr_t>(this) + 0x28); }
	hkHkbBehGraph* getHkbBehGraph() { return *reinterpret_cast<hkHkbBehGraph**>(reinterpret_cast<uintptr_t>(this) + 0x98); }
};
