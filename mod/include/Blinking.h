#pragma once

#include <cmath>

#include "GameRepository.h"

#define BLINK_TIMER_INTERVAL 5.0f
#define BLINK_START_TIME 2.2f
#define BLINK_END_TIME 3.0f

#define BLINK_HOLD_MIN_THRESHOLD 0.1666f
#define BLINK_HOLD_MAX_THRESHOLD 3.5f

struct Brightness;

extern void onBrightnessUpdate(void* instance, Brightness* brightness);
extern bool getBlinkState();