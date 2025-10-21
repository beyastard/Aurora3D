#ifndef _ACOMMON_LOG_H_
#define _ACOMMON_LOG_H_

#include <functional>

// Log level enumeration
enum class LogLevel { Info = 0, Error = 1 };

// Log output function signature
using LogOutputFunc = std::function<void(std::string_view)>;

// Redirect log output (returns previous handler)
LogOutputFunc SetLogOutput(LogOutputFunc handler);

// Template-based logging functions (defined inline in header)
template<typename... Args>
void Log(LogLevel level, std::string_view format, Args&&... args);

template<typename... Args>
void LogInfo(std::string_view format, Args&&... args);

template<typename... Args>
void LogError(std::string_view format, Args&&... args);

template<typename... Args>
void LogRaw(LogLevel level, std::string_view format, Args&&... args);

template<typename... Args>
void SendLogToDebugWindow(LogLevel level, std::string_view format, Args&&... args);

// Safe directory creation
void CreateDirectories(std::string_view path);
bool TryCreateDirectories(std::string_view path);
void CreateDirectoriesWithWinError(std::string_view path);

// Internal helper (implementation detail)
void LogImpl(LogLevel level, std::string_view formattedMessage);

// Template implementations
#include "ACommonLog_impl.h"

#endif
