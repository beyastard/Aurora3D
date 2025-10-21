#ifndef _ATIME_H_
#define _ATIME_H_

void a_GetSystemTime(std::uint16_t* pYear, std::uint16_t* pMonth, std::uint16_t* pDay,
    std::uint16_t* pDate, std::uint16_t* pHour, std::uint16_t* pMinute, std::uint16_t* pSecond);

std::uint32_t a_GetTime();

#endif
