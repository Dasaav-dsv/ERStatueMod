#pragma once

#include <cstdint>
#include <utility>

#define PLAYER_HANDLE 0xFFFFFFFF15A00000ull
#define TORRENT_HANDLE 0xFFFFFFFF15C00000ull

class CSChrIns;

class WorldChrManImp {
public:
	using FnGetInstance = WorldChrManImp*(*)();
	using FnGetMainPlayerIns = CSChrIns*(*)(WorldChrManImp*);

	auto getChrInsVector() {
		return std::pair(*reinterpret_cast<CSChrIns***>(reinterpret_cast<uintptr_t>(this) + 0x1F1B0), 
			*reinterpret_cast<CSChrIns***>(reinterpret_cast<uintptr_t>(this) + 0x1F1B8));
	}
};

class CSFlipper {
public:
	float dT() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0x270);
	}
};

class CSSoundImp {
public:
	using FnIsPlayerInCombat = bool(*)(CSSoundImp*);
};

class hkHkbCharacter;

class CSChrIns {
public:
	bool isPlayerOrTorrent() {
		auto handle = *reinterpret_cast<uint64_t*>(reinterpret_cast<uintptr_t>(this) + 0x8);
		return handle == PLAYER_HANDLE || handle == TORRENT_HANDLE;
	}

	bool isTargetingPlayer() {
		if (this->isPlayerOrTorrent()) return false;
		void* CSChrModules = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(this) + 0x190);
		if (!CSChrModules) return false;
		void* CSChrAiModule = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(CSChrModules) + 0x38);
		if (!CSChrAiModule) return false;
		void* ChrAiData = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(CSChrAiModule) + 0x18);
		if (!ChrAiData) return false;
		auto tgtHandle = *reinterpret_cast<uint64_t*>(reinterpret_cast<uintptr_t>(ChrAiData) + 0x158);
		return tgtHandle == PLAYER_HANDLE || tgtHandle == TORRENT_HANDLE;
	}

	int getHp() {
		void* CSChrModules = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(this) + 0x190);
		if (!CSChrModules) return -1;
		void* CSChrDataModule = *reinterpret_cast<void**>(CSChrModules);
		if (!CSChrDataModule) return -1;
		int hp = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(CSChrDataModule) + 0x138);
		return hp;
	}

	int getMaxHp() {
		void* CSChrModules = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(this) + 0x190);
		if (!CSChrModules) return -1;
		void* CSChrDataModule = *reinterpret_cast<void**>(CSChrModules);
		if (!CSChrDataModule) return -1;
		int maxHp = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(CSChrDataModule) + 0x13C);
		return maxHp;
	}

	bool isDead() {
		int hp = this->getHp();
		return hp <= 0;
	}

	bool isVisible() {
		if (this->isPlayerOrTorrent()) return false;
		auto flags = *reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + 0x1C4);
		return (flags >> 4) & 1;
	}

	void setSpeed(float newSpeed) {
		if (this->isPlayerOrTorrent()) return;
		void* CSChrModules = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(this) + 0x190);
		if (!CSChrModules) return;
		void* CSChrBehModule = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(CSChrModules) + 0x28);
		if (!CSChrBehModule) return;
		float& speed = *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(CSChrBehModule) + 0x17C8);
		speed = newSpeed;
	}

	bool isUseHeldPlayer() {
		void* CSChrModules = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(this) + 0x190);
		if (!CSChrModules) return false;
		void* CSChrActionRequestModule = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(CSChrModules) + 0x80);
		if (!CSChrActionRequestModule) return false;
		auto flags = *reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(CSChrActionRequestModule) + 0x10);
		return (flags >> 4) & 1;
	}

	hkHkbCharacter* getHkbCharacter() {
		void* CSChrModules = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(this) + 0x190);
		if (!CSChrModules) return nullptr;
		void* CSChrBehModule = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(CSChrModules) + 0x28);
		if (!CSChrBehModule) return nullptr;
		void* ChrBehData = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(CSChrBehModule) + 0x10);
		if (!ChrBehData) return nullptr;
		hkHkbCharacter* hkbCharacter = *reinterpret_cast<hkHkbCharacter**>(reinterpret_cast<uintptr_t>(ChrBehData) + 0x30);
		return hkbCharacter;
	}
};

class CSChrActionFlagModule {
public:
	using FnGetMaxTurnSpeed = float(*)(CSChrActionFlagModule*);

	CSChrIns* getOwner() {
		return *reinterpret_cast<CSChrIns**>(reinterpret_cast<uintptr_t>(this) + 0x8);
	}

	float& maxTurnSpeed() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0x84);
	}
};

class CSTeamType {
public:
	using FnGetRelationship = bool(*)(CSChrIns*, CSChrIns*, int, int, int);
};