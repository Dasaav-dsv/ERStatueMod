#pragma once

#include <string>

#include "mINI/ini.h"

template <typename T>
class IniParseLineBase {
public:
	IniParseLineBase(mINI::INIStructure& ini, const char* section, const char* key, const T& defaultValue) : ini(ini), section(section), key(key), value(defaultValue), defaultValue(defaultValue) {}

	virtual T valueFromStr(const char* valueString) const = 0;
	virtual std::string defaultValueToStr() const = 0;

	const char* c_str() const {
		if (this->ini[this->section].has(this->key)) {
			return this->ini[this->section][this->key].data();
		}
		else {
			return nullptr;
		}
	}

	void setDefaultValue() {
		this->ini[this->section][this->key] = this->defaultValueToStr();
	}

	void parse() {
		const char* valueStr = this->c_str();
		if (!valueStr) this->setDefaultValue();
		else this->value = this->valueFromStr(valueStr);
	}

	operator T() const { return this->value; }

protected:
	mINI::INIStructure& ini;
	const char* section;
	const char* key;
	T defaultValue;
	T value;
};

template <typename T>
class IniParseLine;

template <>
class IniParseLine<bool> : public IniParseLineBase<bool> {
public:
	IniParseLine(mINI::INIStructure& ini, const char* section, const char* key, bool defaultValue) : IniParseLineBase(ini, section, key, defaultValue) {
		this->parse();
	}

	bool valueFromStr(const char* valueString) const { return std::strcmp(valueString, "false"); }
	std::string defaultValueToStr() const { return this->defaultValue ? "true" : "false"; }
};

template <>
class IniParseLine<float> : public IniParseLineBase<float> {
public:
	IniParseLine(mINI::INIStructure& ini, const char* section, const char* key, float defaultValue) : IniParseLineBase(ini, section, key, defaultValue) {
		this->parse();
	}

	float valueFromStr(const char* valueString) const { return std::stof(valueString); }
	std::string defaultValueToStr() const { return std::to_string(this->defaultValue); }
};