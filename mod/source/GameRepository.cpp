#include "GameRepository.h"

GameRepository::GameRepository() noexcept {
#ifdef _DEBUG
	Logger::allocateConsole();
#endif

	memset((void*)&this->calls, 0, sizeof(this->calls));
	this->getAddresses();

	{
		auto& globals = this->globals;
		globals.WorldChrManImp = reinterpret_cast<WorldChrManImp**>(this->find(WORLDCHRMANIMP_GLOBAL_SIGNATURE));
		globals.FieldArea = reinterpret_cast<void**>(this->find(FIELDAREA_GLOBAL_SIGNATURE));
		globals.CSFlipper = reinterpret_cast<CSFlipper**>(this->find(CSFLIPPER_GLOBAL_SIGNATURE));
		globals.CSSoundImp = reinterpret_cast<CSSoundImp**>(this->find(CSSOUNDIMP_GLOBAL_SIGNATURE));
	}

	{
		auto& calls = this->calls;
		calls.callsGXRend.updateBrightness = this->find(GXREND_UPDATEBRIGHTNESS_CALL_SIGNATURE);
	}

	{
		auto& functions = this->functions;
		functions.fnWorldChrManImp.getInstance = reinterpret_cast<WorldChrManImp::FnGetInstance>(this->find(WORLDCHRMANIMP_GETINSTANCE_FUNCTION_SIGNATURE));
		functions.fnWorldChrManImp.getMainPlayerIns = reinterpret_cast<WorldChrManImp::FnGetMainPlayerIns>(this->find(WORLDCHRMANIMP_GETPLAYERINS_FUNCTION_SIGNATURE));
		functions.fnCSSoundImp.isPlayerInCombat = reinterpret_cast<CSSoundImp::FnIsPlayerInCombat>(this->find(CSSOUNDIMP_ISPLAYERINCOMBAT_FUNCTION_SIGNATURE));
		functions.fnCSTeamType.getRelationship = reinterpret_cast<CSTeamType::FnGetRelationship>(this->find(CSTEAMTYPE_GETRELATIONSHIP_FUNCTION_SIGNATURE));
	}
}

void GameRepository::getAddresses() noexcept {
	auto& base = this->base;
	base.pHandle = GetCurrentProcess();
	GetModuleInformation(base.pHandle, GetModuleHandleA(NULL), &base.mInfo, sizeof(base.mInfo));
	base.baseAddress = base.mInfo.lpBaseOfDll;
	auto baseAddress = (uintptr_t)base.baseAddress;
	auto dosImage = reinterpret_cast<IMAGE_DOS_HEADER*>(baseAddress);
	auto ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS32*>(baseAddress + dosImage->e_lfanew);
	auto sections = reinterpret_cast<IMAGE_SECTION_HEADER*>((uintptr_t)ntHeaders + sizeof(ntHeaders->Signature) + sizeof(ntHeaders->FileHeader) + ntHeaders->FileHeader.SizeOfOptionalHeader);
	auto numSections = ntHeaders->FileHeader.NumberOfSections;
	for (int i = 0; i < numSections; ++i) {
		auto& section = sections[i];
		auto name64 = *reinterpret_cast<uint64_t*>(&section.Name);
		if (name64 == 0x00'00'00'74'78'65'74'2Eull) {
			auto sectionStart = reinterpret_cast<void*>(baseAddress + section.PointerToRawData);
			auto sectionSize = static_cast<size_t>(section.SizeOfRawData);
			base.textSections.push_back({ sectionStart, sectionSize });
		}
	}
}

void* GameRepository::find(const char* signature, const char* name, int offset, int inset) noexcept {
	void* result = nullptr;
	for (auto& [start, size] : this->base.textSections) {
		if ((result = Pattern16::find(start, size, signature))) break;
	}
	if (!result) {
		Logger::log(Logger::ERR, "Unable to find %s, Pattern16 returned nullptr", name);
		return nullptr;
	}
	auto base = (uintptr_t)this->base.baseAddress;
	result = (void*)((uintptr_t)result + offset);
	if (!inset) {
		Logger::log(Logger::DEBUG, "Found %s at offset %X", name, (uintptr_t)result - (uintptr_t)this->base.baseAddress);
		return result;
	}
	else {
		void* inResult = (void*)((uintptr_t)result + inset);
		intptr_t displacement = *reinterpret_cast<int*>(inResult);
		inResult = (void*)((uintptr_t)inResult + displacement + 4);
		Logger::log(Logger::DEBUG, "Found %s at offset %X, target offset %X", name, (uintptr_t)result - base, (uintptr_t)inResult - base);
		return inResult;
	}
}

GameRepository GLOBAL_GAMEREPOSITORY;