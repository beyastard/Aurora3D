#ifndef _ALOG_H_
#define _ALOG_H_

#include <fstream>
#include <mutex>

class ALog
{
public:
    ALog() = default;
    ~ALog();

    bool Init(std::string_view logFileName, std::string_view helloMsg, bool append = false);
    bool Release();

    template<typename... Args>
    bool Log(std::string_view fmt, Args&&... args) {
        return LogString(std::vformat(fmt, std::make_format_args(args...)));
    }

    bool LogString(std::string_view message);

    static void SetLogDir(std::string_view dir);
    static std::string GetLogDir();

private:
	static std::string m_logDir;
	static std::mutex m_logDirMutex;

	std::ofstream m_file;
	mutable std::mutex m_mutex;
};

typedef ALog* PALog;

#endif
