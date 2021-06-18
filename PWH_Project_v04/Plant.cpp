#include "Plant.h"

constexpr auto g_april{ 4 };
constexpr auto g_september{ 9 };

Plant::Plant(const std::string& name, int id, int in_warm, int in_cold, int water_amount)
	: m_name{ name }, m_id{ id }, m_in_warm{ in_warm }, m_in_cold{ in_cold }, m_water_amount{ water_amount }, m_register_date{}
{
	if (m_register_date.m_month >= g_april && m_register_date.m_month <= g_september)
		m_times = m_in_warm;
	else
		m_times = m_in_cold;

	if (m_times < 1) // every day is minimum
		m_times = 1;

	wateringWasDone();
	updateInfo();
}

void Plant::updateInfo()
{
	Date current{};
	if (current.m_month >= g_april && current.m_month <= g_september) // if it is warm month now
		m_times = m_in_warm;
	else
		m_times = m_in_cold;

	if (m_times < 1) // every day is minimum
		m_times = 1;

	m_days_left = current.getTimeDifferenceInDays(m_next_watering_date);
}

void Plant::wateringWasDone()
{
	Date current{};
	m_next_watering_date.setCurrentTime();

	m_next_watering_date.addDays(m_times);
	m_days_left = current.getTimeDifferenceInDays(m_next_watering_date);
}

int Plant::getID() const { return m_id; }

int Plant::getDaysLeft() const { return m_days_left; }

int Plant::getWaterAmount() const { return m_water_amount; }

const std::string& Plant::getName() const { return m_name; }

std::ostream& operator<<(std::ostream& out, const Plant& plant)
{
	out << plant.m_name << " " << plant.m_register_date << " " << plant.m_id
		<< " " << plant.m_next_watering_date << " " << plant.m_in_warm << " " 
		<< plant.m_in_cold << " " << plant.m_water_amount;

	return out;
}

std::istream& operator>>(std::istream& in, Plant& plant)
{
	in  >> plant.m_name >> plant.m_register_date >> plant.m_id
		>> plant.m_next_watering_date >> plant.m_in_warm 
		>> plant.m_in_cold >> plant.m_water_amount;

	return in;
}
