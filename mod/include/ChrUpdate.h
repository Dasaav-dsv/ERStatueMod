#pragma once

#include <cstdint>

#include "ini/ini.h"

#include "hkTypes.h"
#include "CSTypes.h"

#include "Blinking.h"

extern void onHkbChrUpdate(void* instance, hkHkbCharacter* hkbCharacter);
extern void onGetMaxTurnSpeed(CSChrActionFlagModule* instance);