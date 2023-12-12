#pragma once

#include <cmath>

#include "ini/ini.h"

#include "GameRepository.h"

struct Brightness;

extern void onBrightnessUpdate(void* instance, Brightness* brightness);
extern bool getBlinkState();