#ifndef _AFPI_H_
#define _AFPI_H_

#include "ALog.h"

// Access the file module's error logger
ALog* GetAFErrorLog();

// Get base directory
std::string_view GetAFBaseDir();

// Check if compression is enabled
bool IsAFCompressionEnabled();

// Usage: AFERRLOG("Error at {}", nval);
// Note: Uses C++20 std::format syntax
#define AFERRLOG(fmt, ...) \
    do { \
        ALog* _log = GetAFErrorLog(); \
        if (_log) { \
            _log->Log(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#endif
