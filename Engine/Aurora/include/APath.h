#ifndef _APATH_H_
#define _APATH_H_

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

// Example of usage:
//
// #include "apath.h"
// #include <iostream>
// 
// int main()
// {
//     std::string full = R"(C:\Projects\MyApp\main.cpp)";
//     std::string parent = R"(C:\Projects\)";
//     std::string rel = APath::GetRelativePath(full, parent);
//     std::cout << "Relative: " << rel << "\n"; // prints "MyApp\main.cpp"
// 
//     std::string base = R"(C:\Data)";
//     std::string file = R"(.\config.txt)";
//     std::string fullPath = APath::GetFullPath(base, file);
//     std::cout << "Full: " << fullPath << "\n"; // prints "C:\Data\config.txt"
// 
//     std::string dirty = "  \\path\\to\\file.txt\t ";
//     std::string clean = APath::TrimPath(dirty);
//     std::cout << "Clean: '" << clean << "'\n"; // prints "\path\to\file.txt"
// }
//

namespace APath
{
    // Case-insensitive character comparison (locale-independent, ASCII-safe)
    inline bool iequal(char a, char b)
    {
        return std::tolower(static_cast<unsigned char>(a)) ==
            std::tolower(static_cast<unsigned char>(b));
    }

    // Returns the relative path of 'fullPath' with respect to 'parentPath'
    inline std::string GetRelativePath(std::string_view fullPath, std::string_view parentPath)
    {
        // Handle empty parent
        if (parentPath.empty())
            return std::string(fullPath);

        // Find first mismatch (case-insensitive)
        auto [it1, it2] = std::mismatch(
            parentPath.begin(), parentPath.end(),
            fullPath.begin(), fullPath.end(),
            iequal
        );

        // If we didn't consume all of parentPath, it's not a prefix
        if (it1 != parentPath.end())
            return std::string(fullPath);

        // Skip separator after matched prefix (Windows-style backslash)
        std::string_view remaining = fullPath.substr(it2 - fullPath.begin());
        if (!remaining.empty() && remaining[0] == '\\')
            remaining.remove_prefix(1);

        return std::string(remaining);
    }

    // Constructs full path from base directory and filename
    inline std::string GetFullPath(std::string_view baseDir, std::string_view filename)
    {
        if (filename.empty())
            return {};

        // Absolute path? (Windows: "X:\...")
        if (filename.size() >= 3 && std::isalpha(static_cast<unsigned char>(filename[0])) && filename[1] == ':' && filename[2] == '\\')
            return std::string(filename);

        // Skip leading ".\" if present
        std::string_view realFile = filename;
        if (filename.size() >= 2 && filename.substr(0, 2) == ".\\")
            realFile = filename.substr(2);

        // If baseDir is empty, just return the cleaned filename
        if (baseDir.empty())
            return std::string(realFile);

        // Ensure baseDir ends with backslash
        std::string result = std::string(baseDir);
        if (result.back() != '\\')
            result += '\\';

        result += realFile;

        return result;
    }

    inline std::string TrimPath(std::string_view path)
    {
        auto isSpaceOrTab = [](unsigned char c) { return c == ' ' || c == '\t'; };

        auto start = std::find_if_not(path.begin(), path.end(), isSpaceOrTab);
        if (start == path.end())
            return {}; // all whitespace

        auto end = std::find_if_not(path.rbegin(), path.rend(), isSpaceOrTab).base();

        return std::string(start, end);
    }
}

#endif
