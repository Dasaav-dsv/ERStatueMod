#include "ini/ini.h"

ERStatueIni::ERStatueIni() :
	file(new mINI::INIFile("erstatue_settings.ini")),
	ini(new mINI::INIStructure),
	iniExists(this->file->read(*this->ini)),

	blinkTimerInterval(IniParseLine<float>(*this->ini, "[BLINKING]", "Blink Interval", BLINK_TIMER_INTERVAL)),
	blinkStartTime(IniParseLine<float>(*this->ini, "[BLINKING]", "Blink Start Time", BLINK_START_TIME)),
	blinkEndTime(IniParseLine<float>(*this->ini, "[BLINKING]", "Blink Ent Time", BLINK_END_TIME)),

	blinkHoldMin(IniParseLine<float>(*this->ini, "[HOLD INTERACT BLINK]", "Hold Start Time", BLINK_HOLD_MIN_THRESHOLD)),
	blinkHoldMax(IniParseLine<float>(*this->ini, "[HOLD INTERACT BLINK]", "Hold End Time", BLINK_HOLD_MAX_THRESHOLD)),

	enemySpeed(IniParseLine<float>(*this->ini, "[UNSEEN ENEMY SPEED]", "Normal Enemy Speed", ENEMY_STATUE_SPEED)),
	enemySpeedBlink(IniParseLine<float>(*this->ini, "[UNSEEN ENEMY SPEED]", "Enemy Speed on Blink", ENEMY_STATUE_SPEED_BLINK)),

	blinkLongerHurt(IniParseLine<bool>(*this->ini, "[BLINKING]", "Blink Longer when Hurt", BLINK_LONGER_WHEN_HURT)),
	blinkFastEnemies(IniParseLine<bool>(*this->ini, "[UNSEEN ENEMY SPEED]", "Enemies Change Speed on Blink", ENEMY_FAST_ON_BLINK))
{
	if (this->iniExists) this->file->write(*this->ini, true);
	else this->file->generate(*this->ini, true);

	delete this->file;
	delete this->ini;
}

ERStatueIni GLOBAL_INI;