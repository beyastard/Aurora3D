#ifndef _AFI_H_
#define _AFI_H_

#include <string>
#include <string_view>

namespace AFileMod
{
    // Initialize file module (enables compression if needed)
    bool Initialize(bool compressEnable = true);

    // Set base directory (relative paths are resolved against this)
    bool SetBaseDir(std::string_view baseDir);

    // Finalize and clean up
    bool Finalize();

    // Get current base directory
    std::string GetBaseDir();

    // Full path construction
    std::string GetFullPath(std::string_view folderName, std::string_view fileName);
    std::string GetFullPath(std::string_view fileName);

    // Relative path computation
    std::string GetRelativePath(std::string_view fullPath, std::string_view folderName);
    std::string GetRelativePath(std::string_view fullPath);

    // Extract file title (filename without path)
    std::string GetFileTitle(std::string_view filePath);

    // Extract file path (directory part only)
    std::string GetFilePath(std::string_view filePath);
}

#endif
