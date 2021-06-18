#pragma once

#include "Date.h"
#include <Windows.h>

class Plant
{
private:
	std::string m_name{};

	int m_id          {};
	int m_in_warm     {};
	int m_in_cold     {};
	int m_water_amount{};
	int m_times       {};
	int m_days_left   {};

	Date m_register_date     {};
	Date m_next_watering_date{};

public:
	Plant() = default; // for DataMngr::readFromFile() only
	Plant(const std::string & name, int id, int in_warm, int in_cold, int water_amount);

	void updateInfo();
	void wateringWasDone();
	int getDaysLeft() const;
	int getID() const;
	int getWaterAmount() const;
	const std::string& getName() const;

	friend std::ostream& operator<<(std::ostream& out, const Plant& plant);
	friend std::istream& operator>>(std::istream& in, Plant& plant);
};

