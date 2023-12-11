#include "logger.h"

namespace Logger {
	static std::mutex printMutex;

	inline std::string levelToString(Level level) {
		switch (level) {
		case TRACE:
			return "[TRC]";
		case DEBUG:
			return "[DBG]";
		case INFO:
			return "[INF]";
		case WARN:
			return "[WRN]";
		case ERR:
			return "[ERR]";
		case FATAL:
			return "[FTL]";
		default:
			return "[INV]";
		}
	}

	auto getTime() noexcept {
		std::time_t time = std::time(nullptr);
		char timeString[32];
		std::tm timeInfo;
		localtime_s(&timeInfo, &time);
		std::strftime(timeString, std::size(timeString), "%F_%H-%M-%S", &timeInfo);
		return std::string(timeString);
	}

	auto getLogFile() {
		static auto logFile = []() {
			CreateDirectoryA("log", NULL);
			std::string log = "log\\erstatue";
			CreateDirectoryA(log.c_str(), NULL);
			return _fsopen((log + getTime() + ".log").c_str(), "w", _SH_DENYWR);
		}();
		return logFile;
	}

	void log(Level level, std::string message, ...) {
		va_list args;
		va_start(args, message);
		auto out = levelToString(level) + " " + message + "\n";
		{
			std::scoped_lock lock(printMutex);
			auto logFile = getLogFile();
			vfprintf(logFile, out.c_str(), args);
			vprintf(out.c_str(), args);
			fflush(logFile);
		}
		va_end(args);
	}

	void log(const std::string& log) {
		if (!log.empty()) {
			std::scoped_lock lock(printMutex);
			auto logFile = getLogFile();
			fprintf(logFile, log.c_str());
			printf(log.c_str());
			fflush(logFile);
		}
	}

	bool allocateConsole() {
		static bool _ = []() {
			AllocConsole();
			FILE* out;
			freopen_s(&out, "CON", "w", stdout);
			return true;
			}();
		return _;
	}
}