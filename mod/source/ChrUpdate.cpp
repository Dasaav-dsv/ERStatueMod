#include "ChrUpdate.h"

void onHkbChrUpdate(void* instance, hkHkbCharacter* hkbCharacter) {
	auto WorldChrManImp = *GLOBAL_GAMEREPOSITORY.globals.WorldChrManImp;
	if (!WorldChrManImp) return;
	auto player = GLOBAL_GAMEREPOSITORY.functions.fnWorldChrManImp.getMainPlayerIns(WorldChrManImp);
	if (!player) return;
	auto pChrIns = hkbCharacter->getChrIns();
	if (!pChrIns) return;
	bool& updateState = hkbCharacter->getHkbBehGraph()->getUpdateState();
	bool isEnemy = GLOBAL_GAMEREPOSITORY.functions.fnCSTeamType.getRelationship(player, pChrIns, 1, 0, 0);
	if (!isEnemy) {
		updateState = true;
		return;
	}
	bool isPlayerBlinking = getBlinkState();
	bool isDead = pChrIns->isDead();
	bool isInvisible = !pChrIns->isVisible() || isPlayerBlinking;
	float statueSpeed = isPlayerBlinking ? ENEMY_STATUE_SPEED_BLINK : ENEMY_STATUE_SPEED;
	pChrIns->setSpeed(isInvisible && !isDead ? statueSpeed : 1.0f);
	updateState = isInvisible || isDead;
}

void onGetMaxTurnSpeed(CSChrActionFlagModule* instance) {
	auto owner = instance->getOwner();
	auto hkbCharacter = owner->getHkbCharacter();
	if (!hkbCharacter) return;
	bool updateState = hkbCharacter->getHkbBehGraph()->getUpdateState();
	float& maxTurnSpeed = instance->maxTurnSpeed();
	if (!updateState) maxTurnSpeed = 0.0f;
}