#include "Blinking.h"

struct Brightness {
	float invBrightnessActual;
	float invBrightness2;
	float invBrightness3;
	float brightnessActual;
	float brightness2;
	float brightness3;
};

bool blinkState = false;

bool isPlayerInCombat() {
	auto CSSoundImp = *GLOBAL_GAMEREPOSITORY.globals.CSSoundImp;
	if (!CSSoundImp) return false;
	return GLOBAL_GAMEREPOSITORY.functions.fnCSSoundImp.isPlayerInCombat(CSSoundImp);
}

bool isPlayerTargeted() {
	auto WorldChrManImp = *GLOBAL_GAMEREPOSITORY.globals.WorldChrManImp;
	if (!WorldChrManImp) return false;
	auto [cur, end] = WorldChrManImp->getChrInsVector();
	while (cur != end) {
		if ((*cur)->isTargetingPlayer()) return true;
		++cur;
	}
	return false;
}

bool isPlayerDead() {
	auto WorldChrManImp = *GLOBAL_GAMEREPOSITORY.globals.WorldChrManImp;
	if (!WorldChrManImp) return true;
	auto player = GLOBAL_GAMEREPOSITORY.functions.fnWorldChrManImp.getMainPlayerIns(WorldChrManImp);
	if (!player) return true;
	float hp = player->getHp();
	return hp <= 0;
}

bool getBlinkState() {
	bool blinkCondition = isPlayerInCombat() || isPlayerTargeted();
	return blinkState && blinkCondition;
}

void setBlinkState(bool newState) {
	blinkState = newState;
}

constexpr float getBlinkTimerInterval() {
	return BLINK_TIMER_INTERVAL;
}

constexpr float getBlinkStartTime() {
	return BLINK_START_TIME;
}

constexpr float getBlinkTimespan() {
	float span = BLINK_END_TIME - BLINK_START_TIME;
	auto WorldChrManImp = *GLOBAL_GAMEREPOSITORY.globals.WorldChrManImp;
	if (!WorldChrManImp) return span;
	auto player = GLOBAL_GAMEREPOSITORY.functions.fnWorldChrManImp.getMainPlayerIns(WorldChrManImp);
	if (!player) return span;
	float maxHp = player->getMaxHp();
	float currentHp = player->getHp();
	float ratio = currentHp / maxHp;
	span /= (ratio * ratio + 1.0f);
	span *= 2.0f;
	return span;
}

constexpr float getBlinkEndTime() {
	return BLINK_START_TIME + getBlinkTimespan();
}

float getBlinkLength(float timer) {
	return (timer - getBlinkStartTime()) / getBlinkTimespan();
}

bool isBlinkTime(float timer) {
	return (timer >= getBlinkStartTime() && timer < getBlinkEndTime());
}

bool isUseHeldPlayer() {
	static float holdTime = 0.0f;
	auto WorldChrManImp = *GLOBAL_GAMEREPOSITORY.globals.WorldChrManImp;
	if (!WorldChrManImp) return false;
	auto player = GLOBAL_GAMEREPOSITORY.functions.fnWorldChrManImp.getMainPlayerIns(WorldChrManImp);
	if (!player) return false;
	float dT = (*GLOBAL_GAMEREPOSITORY.globals.CSFlipper)->dT();
	if (player->isUseHeldPlayer()) holdTime += dT;
	else holdTime = 0.0f;
	if (holdTime > BLINK_HOLD_MAX_THRESHOLD + getBlinkTimespan()) holdTime = 0.0f;
	return holdTime >= BLINK_HOLD_MIN_THRESHOLD && holdTime < BLINK_HOLD_MAX_THRESHOLD;
}

float brightnessOf(float blinkLength) {
	float twosinpi = std::sinf(blinkLength * 3.1415926535898f) * 2.0f;
	float brightness = 1.0f - std::fmin(twosinpi, 1.0f);
	return brightness;
}

float getBlinkBrightness(float timer) {
	if (!isBlinkTime(timer) || isPlayerDead()) return 1.0f;
	float blinkLength = getBlinkLength(timer);
	return brightnessOf(blinkLength);
}

float updateBlinkTimer(float blinkTimerInterval = getBlinkTimerInterval()) {
	static float timer = 0.0f;
	bool blinkCondition = isPlayerInCombat() || isPlayerTargeted();
	if (isUseHeldPlayer() && blinkCondition) {
		timer = getBlinkStartTime();
	}
	else if (timer > 0.0f || blinkCondition) {
		timer += (*GLOBAL_GAMEREPOSITORY.globals.CSFlipper)->dT();
	}
	if (timer > blinkTimerInterval) timer = 0.0f;
	return timer;
}

void onBrightnessUpdate(void* instance, Brightness* brightness) {
	auto timer = updateBlinkTimer();
	float blinkBrightness = getBlinkBrightness(timer);
	setBlinkState(blinkBrightness <= 0.0f);
	brightness->brightnessActual *= blinkBrightness;
}