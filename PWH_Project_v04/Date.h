#pragma once

#include <iostream>

class Date
{
public:
    int m_year;
    int m_month;
    int m_day;

public:
    Date();
    Date(int year, int month, int day);

    void setCurrentTime();
    void addDays(int days);
    int countLeapYears() const;
    int getDaysInMonth() const;
    int getTimeDifferenceInDays(const Date& timer);
};

std::ostream& operator<<(std::ostream& out, const Date& timer);
std::istream& operator>>(std::istream& in, Date& timer);