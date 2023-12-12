#pragma once

#include "defaults.h"
#include "lineparser.h"

class ERStatueIni {
	mINI::INIFile* file;
	mINI::INIStructure* ini;
	bool iniExists;
public:
	ERStatueIni();

	const float blinkTimerInterval;
	const float blinkStartTime;
	const float blinkEndTime;

	const float blinkHoldMin;
	const float blinkHoldMax;

	const float enemySpeed;
	const float enemySpeedBlink;

	const bool blinkLongerHurt;
	const bool blinkFastEnemies;
};

extern ERStatueIni GLOBAL_INI;