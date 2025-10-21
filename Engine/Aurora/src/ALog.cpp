#include "ALog.h"
#include "ATime.h"

#include <Windows.h>
#include <format>

std::string ALog::m_logDir = "Logs";
std::mutex ALog::m_logDirMutex{};

ALog::~ALog()
{
    Release();
}

bool ALog::Init(std::string_view logFileName, std::string_view helloMsg, bool append)
{
    namespace fs = std::filesystem;

    {
        std::lock_guard lock(m_logDirMutex);
        fs::create_directories(m_logDir);
    }

    auto path = fs::path(m_logDir) / fs::path(logFileName);
    m_file.open(path, append ? std::ios::app : std::ios::out);

    if (!m_file.is_open())
        return false;

    std::uint16_t year, month, dayOfWeek, dayOfMonth, hour, minute, second;
    a_GetSystemTime(&year, &month, &dayOfWeek, &dayOfMonth, &hour, &minute, &second);

    std::ostringstream oss;
    oss << helloMsg << '\n'
        << "Created(or opened) on "
        << std::setfill('0') << std::setw(2) << dayOfMonth << '/'
        << std::setfill('0') << std::setw(2) << month << '/'
        << year << ' '
        << std::setfill('0') << std::setw(2) << hour << ':'
        << std::setfill('0') << std::setw(2) << minute << ':'
        << std::setfill('0') << std::setw(2) << second
        << "\n\n";

    {
        std::lock_guard lock(m_mutex);
        m_file << oss.str() << std::flush;
    }

    return true;
}

bool ALog::Release()
{
    LogString("Log file closed successfully!");

    if (m_file.is_open())
        m_file.close();

    return true;
}

bool ALog::LogString(std::string_view message)
{
    if (!m_file.is_open()) {
        return false;
    }

    std::uint16_t hour, minute, second;
    a_GetSystemTime(nullptr, nullptr, nullptr, nullptr, &hour, &minute, &second);

    std::ostringstream oss;
    oss << '['
        << std::setfill('0') << std::setw(2) << hour << ':'
        << std::setfill('0') << std::setw(2) << minute << ':'
        << std::setfill('0') << std::setw(2) << second
        << "] " << message << '\n';

    {
        std::lock_guard lock(m_mutex);
        m_file << oss.str() << std::flush;
    }

    return true;
}

void ALog::SetLogDir(std::string_view dir)
{
    namespace fs = std::filesystem;
    std::string cleanDir(dir);

    if (!cleanDir.empty() && cleanDir.back() == '\\')
        cleanDir.pop_back();

    {
        std::lock_guard lock(m_logDirMutex);
        m_logDir = cleanDir;
        fs::create_directories(m_logDir);
    }
}

std::string ALog::GetLogDir()
{
    std::lock_guard lock(m_logDirMutex);
    return m_logDir;
}
