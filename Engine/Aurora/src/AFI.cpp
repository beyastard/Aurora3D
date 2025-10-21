#include "pch.h"
#include "AFI.h"
#include "ALog.h"
#include "APath.h"

#include <cstring>

namespace AFileMod
{
    namespace fs = std::filesystem;

    std::string g_baseDir;
    std::unique_ptr<ALog> g_errorLog;
    bool g_compressEnable = false;

    bool AFileMod::Initialize(bool compressEnable)
    {
        Finalize(); // clean up if already initialized

        g_compressEnable = compressEnable;

        // Set base dir to current working directory
        g_baseDir = fs::current_path().string();

        // Remove trailing backslash (Windows)
        if (!g_baseDir.empty() && g_baseDir.back() == '\\')
            g_baseDir.pop_back();

        // Initialize error log
        g_errorLog = std::make_unique<ALog>();
        if (!g_errorLog->Init("AF.log", "Aurora File Module Error Log"))
        {
            g_errorLog.reset();
            return false;
        }

        return true;
    }

    bool SetBaseDir(std::string_view baseDir)
    {
        g_baseDir = baseDir;

        // Normalize: remove trailing backslash
        if (!g_baseDir.empty() && g_baseDir.back() == '\\')
            g_baseDir.pop_back();

        return true;
    }

    bool Finalize()
    {
        if (g_errorLog)
        {
            g_errorLog->Release();
            g_errorLog.reset();
        }

        g_baseDir.clear();

        return true;
    }

    std::string GetBaseDir()
    {
        return g_baseDir;
    }

    // Full path: baseDir + folder + file
    std::string GetFullPath(std::string_view folderName, std::string_view fileName)
    {
        std::string baseWithFolder = APath::GetFullPath(g_baseDir, folderName);
        return APath::GetFullPath(baseWithFolder, fileName);
    }

    // Full path: baseDir + file
    std::string GetFullPath(std::string_view fileName)
    {
        return APath::GetFullPath(g_baseDir, fileName);
    }

    // Relative path: fullPath relative to (baseDir + folder)
    std::string GetRelativePath(std::string_view fullPath, std::string_view folderName)
    {
        std::string baseWithFolder = APath::GetFullPath(g_baseDir, folderName);
        return APath::GetRelativePath(fullPath, baseWithFolder);
    }

    // Relative path: fullPath relative to baseDir
    std::string GetRelativePath(std::string_view fullPath)
    {
        return APath::GetRelativePath(fullPath, g_baseDir);
    }

    // Extract filename (everything after last \ or /)
    std::string GetFileTitle(std::string_view filePath)
    {
        if (filePath.empty())
            return {};

        // Find last separator
        auto lastSlash = filePath.find_last_of("\\/");
        if (lastSlash == std::string_view::npos)
            return std::string(filePath);

        return std::string(filePath.substr(lastSlash + 1));
    }

    // Extract path (everything up to last \ or /)
    std::string GetFilePath(std::string_view filePath)
    {
        if (filePath.empty())
            return {};

        auto lastSlash = filePath.find_last_of("\\/");
        if (lastSlash == std::string_view::npos)
            return {}; // no path, just filename

        return std::string(filePath.substr(0, lastSlash));
    }
}

ALog* GetAFErrorLog()
{
    return AFileMod::g_errorLog.get();
}

std::string_view GetAFBaseDir()
{
    return AFileMod::g_baseDir;
}

bool IsAFCompressionEnabled()
{
    return AFileMod::g_compressEnable;
}
