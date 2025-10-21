#include <format>

template<typename... Args>
void Log(LogLevel level, std::string_view format, Args&&... args)
{
    std::string prefix = (level == LogLevel::Error) ? "<!> " : "<-> ";
    std::string message = prefix + std::vformat(format, std::make_format_args(args...)) + '\n';
    LogImpl(level, message);
}

template<typename... Args>
void LogInfo(std::string_view format, Args&&... args)
{
    Log(LogLevel::Info, format, args...);
}

template<typename... Args>
void LogError(std::string_view format, Args&&... args)
{
    Log(LogLevel::Error, format, args...);
}

template<typename... Args>
void LogRaw(LogLevel level, std::string_view format, Args&&... args)
{
    std::string prefix = (level == LogLevel::Error) ? "<!> " : "<-> ";
    std::string message = prefix + std::vformat(format, std::make_format_args(args...));
    LogImpl(level, message);
}

template<typename... Args>
void SendLogToDebugWindow(LogLevel level, std::string_view format, Args&&... args)
{
    HWND debugWindow = ::FindWindowA(nullptr, "Aurora Debug Window");
    if (!debugWindow)
        return;

    std::string message = std::vformat(format, std::make_format_args(args...));

    COPYDATASTRUCT copyData{};
    copyData.dwData = static_cast<DWORD>(static_cast<int>(level));
    copyData.cbData = static_cast<DWORD>(message.size());
    copyData.lpData = const_cast<char*>(message.data());

    ::SendMessageA(debugWindow, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&copyData));
}
