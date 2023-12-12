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

bool isEnemy(CSChrIns* player, CSChrIns* other) {
	if (!player || other == player) return false;
	bool isFriend = GLOBAL_GAMEREPOSITORY.functions.fnCSTeamType.getRelationship(player, other, 0, 1, 0);
	bool isEnemy = GLOBAL_GAMEREPOSITORY.functions.fnCSTeamType.getRelationship(player, other, 1, 0, 0);
	bool isAlive = other->isAlive();
	return !isFriend && isEnemy && isAlive;
}

bool isPlayerSensed() {
	auto WorldChrManImp = *GLOBAL_GAMEREPOSITORY.globals.WorldChrManImp;
	if (!WorldChrManImp) return false;
	auto player = GLOBAL_GAMEREPOSITORY.functions.fnWorldChrManImp.getMainPlayerIns(WorldChrManImp);
	if (!player) return false;
	auto [cur, end] = WorldChrManImp->getChrInsVector();
	while (cur != end) {
		auto chr = *cur;
		if (isEnemy(player, chr)) {
			bool targetsPlayer = chr->isTargetingPlayer();
			bool isNearPlayer = chr->isCloseTo(player);
			if (targetsPlayer || isNearPlayer) return true;
		}
		++cur;
	}
	return false;
}

bool isPlayerDead() {
	auto WorldChrManImp = *GLOBAL_GAMEREPOSITORY.globals.WorldChrManImp;
	if (!WorldChrManImp) return true;
	auto player = GLOBAL_GAMEREPOSITORY.functions.fnWorldChrManImp.getMainPlayerIns(WorldChrManImp);
	if (!player) return true;
	int hp = player->getHp();
	return hp <= 0;
}

bool getBlinkState() {
	return blinkState;
}

void setBlinkState(bool newState) {
	blinkState = newState;
}

float getBlinkTimerInterval() {
	return GLOBAL_INI.blinkTimerInterval;
}

float getBlinkStartTime() {
	return GLOBAL_INI.blinkStartTime;
}

constexpr float getBlinkTimespan() {
	float span = GLOBAL_INI.blinkEndTime - GLOBAL_INI.blinkStartTime;
	if (!GLOBAL_INI.blinkLongerHurt) return span;
	auto WorldChrManImp = *GLOBAL_GAMEREPOSITORY.globals.WorldChrManImp;
	if (!WorldChrManImp) return span;
	auto player = GLOBAL_GAMEREPOSITORY.functions.fnWorldChrManImp.getMainPlayerIns(WorldChrManImp);
	if (!player) return span;
	float maxHp = static_cast<float>(player->getMaxHp());
	float currentHp = static_cast<float>(player->getHp());
	float ratio = currentHp / maxHp;
	span /= (ratio * ratio + 1.0f);
	span *= 2.0f;
	return span;
}

constexpr float getBlinkEndTime() {
	return GLOBAL_INI.blinkStartTime + getBlinkTimespan();
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
	if (holdTime > GLOBAL_INI.blinkHoldMax + getBlinkTimespan()) holdTime = 0.0f;
	return holdTime >= GLOBAL_INI.blinkHoldMin && holdTime < GLOBAL_INI.blinkHoldMax;
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
	bool blinkCondition = isPlayerInCombat() || isPlayerSensed();
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