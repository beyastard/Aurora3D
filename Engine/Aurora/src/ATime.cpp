#include "pch.h"
#include "ATime.h"

#include <chrono>

struct SystemTimeInfo
{
    std::uint16_t year;
    std::uint16_t month;
    std::uint16_t day_of_week;  // 0 = Sunday
    std::uint16_t day_of_month;
    std::uint16_t hour;
    std::uint16_t minute;
    std::uint16_t second;
};

static SystemTimeInfo GetCurrentSystemTimeInfo()
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    return
    {
        st.wYear,
        st.wMonth,
        st.wDayOfWeek,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond
    };
}

static std::uint64_t GetSteadyMilliseconds()
{
    return GetTickCount64(); // Windows-only, 64-bit, monotonic
}

void a_GetSystemTime(std::uint16_t* pYear, std::uint16_t* pMonth, std::uint16_t* pDay,
    std::uint16_t* pDate, std::uint16_t* pHour, std::uint16_t* pMinute, std::uint16_t* pSecond)
{
    auto t = GetCurrentSystemTimeInfo();

    if (pYear)   *pYear   = static_cast<std::uint16_t>(t.year);
    if (pMonth)  *pMonth  = static_cast<std::uint16_t>(t.month);
    if (pDay)    *pDay    = static_cast<std::uint16_t>(t.day_of_week);  // pDay = day of week
    if (pDate)   *pDate   = static_cast<std::uint16_t>(t.day_of_month); // pDate = day of month
    if (pHour)   *pHour   = static_cast<std::uint16_t>(t.hour);
    if (pMinute) *pMinute = static_cast<std::uint16_t>(t.minute);
    if (pSecond) *pSecond = static_cast<std::uint16_t>(t.second);
}

std::uint32_t a_GetTime()
{
    return static_cast<std::uint32_t>(GetSteadyMilliseconds()); // Truncate to 32-bit
}
