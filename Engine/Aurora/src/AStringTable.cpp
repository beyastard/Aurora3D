#include "pch.h"
#include "AStringTable.h"
#include "AScriptFile.h"
#include "AFileImage.h"
#include "AFPI.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace
{
    // Helper: convert string to uppercase
    std::string ToUpper(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
        return str;
    }

    // Helper: case-insensitive hash for unordered_map
    struct CaseInsensitiveHash
    {
        std::size_t operator()(const std::string& key) const
        {
            std::string upperKey = ToUpper(key);
            return std::hash<std::string>{}(upperKey);
        }
    };

    // Helper: case-insensitive equality for unordered_map
    struct CaseInsensitiveEqual
    {
        bool operator()(const std::string& lhs, const std::string& rhs) const
        {
            return ToUpper(lhs) == ToUpper(rhs);
        }
    };
}

bool AStringTable::Init(std::string_view filename)
{
    try
    {
        AFileImage fileImage;
        if (!fileImage.Open(filename, AFILE_OPENEXIST | AFILE_BINARY))
        {
            AFERRLOG("AStringTable::Init(), Can not open file [{}]", filename);
            return false;
        }

        AScriptFile scriptFile;
        if (!scriptFile.Open(&fileImage))
        {
            AFERRLOG("AStringTable::Init(), Can not open script file [{}]", filename);
            return false;
        }

        // Clear existing data
        m_entries.clear();
        m_nameToIndex.clear();

        // Parse all entries
        while (scriptFile.GetNextToken(true))
        {
            std::string entryName = scriptFile.GetCurrentToken();
            if (!scriptFile.GetNextToken(true))
            {
                AFERRLOG("AStringTable::Init(), Missing entry data for [{}]", entryName);
                return false;
            }

            std::string entryData = scriptFile.GetCurrentToken();

            if (!AddEntry(entryName, entryData))
            {
                AFERRLOG("AStringTable::Init(), Call AddEntry() failed for [{}]", entryName);
                return false;
            }
        }

        m_hasSorted = false;

        return true;
    }
    catch (const std::exception& e)
    {
        AFERRLOG("AStringTable::Init(), Exception: {}", e.what());
        return false;
    }
}

bool AStringTable::Release()
{
    m_entries.clear();
    m_nameToIndex.clear();
    m_hasSorted = false;

    return true;
}

bool AStringTable::GetEntry(std::string_view entryName, std::string& entryData) const
{
    std::string nameStr(entryName);
    auto it = m_nameToIndex.find(nameStr);
    if (it != m_nameToIndex.end())
    {
        entryData = m_entries[it->second].data;
        return true;
    }

    return false;
}

bool AStringTable::GetEntryDataByIndex(size_t index, std::string& entryData) const
{
    if (index >= m_entries.size())
        return false;
    
    entryData = m_entries[index].data;

    return true;
}

bool AStringTable::GetEntryNameByIndex(size_t index, std::string& entryName) const
{
    if (index >= m_entries.size())
        return false;
    
    entryName = m_entries[index].name;

    return true;
}

bool AStringTable::AddEntry(std::string_view entryName, std::string_view entryData)
{
    try
    {
        std::string entryNameStr(entryName);
        std::string nameStr = ToUpper(entryNameStr);
        std::string dataStr(entryData);

        // Check if entry already exists
        if (m_nameToIndex.find(entryNameStr) != m_nameToIndex.end())
        {
            // Update existing entry
            size_t index = m_nameToIndex[entryNameStr];
            m_entries[index].data = dataStr;
        }
        else
        {
            // Add new entry
            size_t index = m_entries.size();
            m_entries.push_back({ nameStr, dataStr });
            m_nameToIndex[entryNameStr] = index; // Original case for lookup
        }

        m_hasSorted = false;

        return true;
    }
    catch (const std::exception& e)
    {
        AFERRLOG("AStringTable::AddEntry(), Exception: {}", e.what());
        return false;
    }
}

bool AStringTable::ResortEntries()
{
    // Sort entries by name (case-insensitive)
    std::sort(m_entries.begin(), m_entries.end(),
        [](const StringEntry& a, const StringEntry& b) {
            return ToUpper(a.name) < ToUpper(b.name);
        });

    // Rebuild index map
    m_nameToIndex.clear();
    for (size_t i = 0; i < m_entries.size(); ++i)
        m_nameToIndex[m_entries[i].name] = i;

    m_hasSorted = true;

    return true;
}

int AStringTable::CompareTwoEntries(size_t index1, size_t index2) const
{
    if (index1 >= m_entries.size() || index2 >= m_entries.size())
        throw std::out_of_range("Entry index out of bounds");

    std::string name1 = ToUpper(m_entries[index1].name);
    std::string name2 = ToUpper(m_entries[index2].name);

    if (name1 < name2)
        return -1;

    if (name1 > name2)
        return 1;

    return 0;
}
