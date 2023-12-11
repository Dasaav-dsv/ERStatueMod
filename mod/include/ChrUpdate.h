#pragma once

#include <cstdint>

#include "hkTypes.h"
#include "CSTypes.h"

#include "Blinking.h"

#define ENEMY_STATUE_SPEED 2.5f
#define ENEMY_STATUE_SPEED_BLINK 4.0f

extern void onHkbChrUpdate(void* instance, hkHkbCharacter* hkbCharacter);