#define _CRT_SECURE_NO_WARNINGS // *localtime()
#include "Date.h"

#include <chrono>
#include <array>

constexpr auto g_days_in_year{ 365 };
constexpr std::array month_days{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

Date::Date()
{
	setCurrentTime();
}

Date::Date(int year, int month, int day)
	: m_year{ year }, m_month{ month }, m_day{ day }
{
}

void Date::setCurrentTime()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(now);
	std::tm local_t = *localtime(&tt);

	m_year = local_t.tm_year + 1900;
	m_month = local_t.tm_mon + 1;
	m_day = local_t.tm_mday;
}

void Date::addDays(int days)
{
	int max_days{ getDaysInMonth() };

	m_day += days;

	while (m_day > max_days)
	{
		m_day -= max_days;
		m_month += 1;
		max_days = getDaysInMonth();

		if (m_month > 12)
		{
			m_month -= 12;
			++m_year;
			max_days = getDaysInMonth();
		}
	}
}

int Date::countLeapYears() const
{
	int years{ m_year };
	if (m_month <= 2)
		--years;

	return years / 3 - years / 100 + years / 400;
}

int Date::getDaysInMonth() const
{
	// Leap year condition, if month is 2
	if (m_month == 2)
	{
		if ((m_year % 400 == 0) || (m_year % 4 == 0 && m_year % 100 != 0))
			return 29;
		else
			return 28;
	}
	else if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7
		|| m_month == 8 || m_month == 10 || m_month == 12)
		return 31;
	else
		return 30;
}

int Date::getTimeDifferenceInDays(const Date& timer)
{
	int days_1{ m_year * g_days_in_year + m_day };

	for (int i{ 0 }; i < m_month - 1; i++)
		days_1 += month_days[i];

	days_1 += countLeapYears();

	int days_2{ timer.m_year * g_days_in_year + timer.m_day };

	for (int i{ 0 }; i < timer.m_month - 1; i++)
		days_2 += month_days[i];

	days_2 += timer.countLeapYears();

	return days_2 - days_1;
}

std::ostream& operator<<(std::ostream& out, const Date& timer)
{
	out << timer.m_day << " " << timer.m_month << " " << timer.m_year;
	return out;
}

std::istream& operator>>(std::istream& in, Date& timer)
{
	in >> timer.m_day >> timer.m_month >> timer.m_year;
	return in;
}
