#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <vector>
#include <memory>

#include "logger.h"
#include "Pattern16/Pattern16.h"

#include "signatures.h"
#include "CSTypes.h"

class GameRepository {
	struct Base {
		void* baseAddress;
		std::vector<std::pair<void*, size_t>> textSections;
		HANDLE pHandle;
		MODULEINFO mInfo;
	};

	struct Globals {
		WorldChrManImp** WorldChrManImp;
		void** FieldArea;
		CSFlipper** CSFlipper;
		CSSoundImp** CSSoundImp;
	};

	struct FnCalls {
		struct CallsGXRend {
			void* updateBrightness;
		} callsGXRend;
	};

	struct Functions {
		struct FnWorldChrManImp {
			WorldChrManImp::FnGetInstance getInstance;
			WorldChrManImp::FnGetMainPlayerIns getMainPlayerIns;
		} fnWorldChrManImp;

		struct FnCSSoundImp {
			CSSoundImp::FnIsPlayerInCombat isPlayerInCombat;
		} fnCSSoundImp;

		struct FnCSTeamType {
			CSTeamType::FnGetRelationship getRelationship;
		} fnCSTeamType;
	};

public:
	GameRepository() noexcept;
	virtual ~GameRepository() = default;

	void getAddresses() noexcept;
	void* find(const char* signature, const char* name, int offset = 0, int inset = 0) noexcept;

	template <typename T>
	static inline T toFnAddress(void* callAddress) noexcept {
		if (!callAddress) return nullptr;
		auto address = (uintptr_t)callAddress + 1;
		intptr_t displacement = *reinterpret_cast<int*>(address);
		return reinterpret_cast<T>(address + displacement + 4);
	}

	Base base;
	Globals globals;
	FnCalls calls;
	Functions functions;

	GameRepository(const GameRepository&) = delete;
	GameRepository& operator = (const GameRepository&) = delete;
};

extern GameRepository GLOBAL_GAMEREPOSITORY;