#include "pch.h"
#include "ACommonLog.h"

#include <mutex>
#include <optional>

namespace
{
    // Global state
    LogOutputFunc g_logHandler = nullptr;
    std::mutex g_logMutex;
}

LogOutputFunc SetLogOutput(LogOutputFunc handler)
{
    std::lock_guard lock(g_logMutex);
    LogOutputFunc old = g_logHandler;
    g_logHandler = std::move(handler);

    return old;
}

void LogImpl(LogLevel level, std::string_view formattedMessage)
{
    std::lock_guard lock(g_logMutex);

    if (g_logHandler)
    {
        g_logHandler(formattedMessage);
        return;
    }

    // Default: Windows debug output
    ::OutputDebugStringA(std::string(formattedMessage).c_str());
}

void CreateDirectories(std::string_view path)
{
    try
    {
        std::filesystem::create_directories(std::filesystem::path(path));
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        LogError("Failed to create directories '{}': {}", path, e.what());
    }
    catch (const std::exception& e)
    {
        LogError("Failed to create directories '{}': {}", path, e.what());
    }
    catch (...)
    {
        LogError("Failed to create directories '{}': Unknown error", path);
    }
}

bool TryCreateDirectories(std::string_view path)
{
    try
    {
        return std::filesystem::create_directories(std::filesystem::path(path));
    }
    catch (...)
    {
        LogError("Failed to create directories '{}'", path);
        return false;
    }
}

void CreateDirectoriesWithWinError(std::string_view path)
{
    try
    {
        std::filesystem::create_directories(std::filesystem::path(path));
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        auto errCode = e.code();
        std::optional<DWORD> winError;
        if (errCode.category() == std::system_category())
            winError = static_cast<DWORD>(errCode.value());

        if (winError.has_value())
            LogError("CreateDirectories failed for '{}': {} (WinError {})", path, e.what(), winError.value());
        else
            LogError("CreateDirectories failed for '{}': {}", path, e.what());
    }
    catch (const std::exception& e)
    {
        LogError("CreateDirectories failed for '{}': {}", path, e.what());
    }
    catch (...)
    {
        LogError("CreateDirectories failed for '{}': Unknown error", path);
    }
}
