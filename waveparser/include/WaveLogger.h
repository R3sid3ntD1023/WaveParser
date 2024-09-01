#pragma once
#include <format>

struct Logger
{
	
	enum LogLevel
	{
		info,
		trace,
		warn,
		error
	};

	typedef void(*LOGCALLBACK)(LogLevel, const char*);

	template<typename... Args>
	static void Log(LogLevel level, const char* message, const Args&... args);

	static void SetCallback(LOGCALLBACK callback)
	{
		sCallback = callback;
	}

private:
	static inline LOGCALLBACK sCallback;
};

template<typename ...Args>
inline void Logger::Log(LogLevel level, const char* message, const Args & ...args)
{
	if (!sCallback) return;

	auto text = std::vformat(message, std::make_format_args(args...));
	sCallback(level, text.c_str());
}

#if _DEBUG
/*levels - info, trace, warn, error*/
#define WAVE_LOG(level, ...) Logger::Log(Logger::LogLevel::level, __VA_ARGS__)
#else
#define WAVE_LOG(level, ...)
#endif