//-----------------------------------------------------------------------
//  File        : TimeStorage.hpp
//  Created     : 11.12.2022
//  Modified    : 11.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Класс для хранения времени

#ifndef TIMECONTAINER
#define TIMECONTAINER

#include <stdint.h>

class TimeContainer {
private:
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;

public:
	TimeContainer(uint8_t aHour, uint8_t aMinutes, uint8_t aSeconds = 0) :
	    hours{aHour},
	    minutes{aMinutes},
	    seconds{aSeconds}
	{
	}

	TimeContainer() :
		hours{0},
		minutes{0},
		seconds{0}
	{
	}

	bool operator == (const TimeContainer &cont1) const&
	{
		return ((hours == cont1.hours) && (minutes == cont1.minutes) && (seconds == cont1.seconds));
	}

	bool operator != (const TimeContainer &cont1) const&
	{
		return ((hours != cont1.hours) || (minutes != cont1.minutes) || (seconds != cont1.seconds));
	}

	bool operator < (const TimeContainer &cont1) const&
	{
		if (hours < cont1.hours) {
			return true;
		} else if (hours > cont1.hours) {
			return false;
		} else if (hours == cont1.hours) {
			if (minutes < cont1.minutes) {
				return true;
			} else if (minutes > cont1.minutes) {
				return false;
			} else if (minutes == cont1.minutes) {
				if (seconds < cont1.seconds) {
					return true;
				} else {
					return false;
				}
			}
		}
		return false;
	}

	bool operator > (const TimeContainer &cont1) const&
	{
		if (hours > cont1.hours) {
			return true;
		} else if (hours < cont1.hours) {
			return false;
		} else if (hours == cont1.hours) {
			if (minutes > cont1.minutes) {
				return true;
			} else if (minutes < cont1.minutes) {
				return false;
			} else if (minutes == cont1.minutes) {
				if (seconds > cont1.seconds) {
					return true;
				} else {
					return false;
				}
			}
		}
		return false;
	}

	void setTime(uint8_t aHour, uint8_t aMinutes, uint8_t aSeconds = 0)
	{
	    hours = aHour;
	    minutes = aMinutes;
	    seconds = aSeconds;
	}

	void setHour(uint8_t aHour)
	{
		hours = aHour;
	}

	void setMinutes(uint8_t aMinutes)
	{
		minutes = aMinutes;
	}

	void getTime(uint8_t &aHours, uint8_t &aMinutes, uint8_t &aSeconds) 
	{
		aHours = hours;
		aMinutes = minutes;
		aSeconds = seconds;
	}

	uint8_t hour() const
	{
		return hours;
	}

	uint8_t minute() const
	{
		return minutes;
	}

	uint8_t second() const
	{
		return seconds;
	}

	void clear()
	{
		hours = 0xFF;
		minutes = 0xFF;
		seconds = 0xFF;
	}

	bool isEmpty()
	{
		return hours == 0xFF && minutes == 0xFF && seconds == 0xFF;
	}

	void addMinutes(uint8_t aMinutes)
	{
		uint8_t curMinutes = minutes;
		if (curMinutes + aMinutes < 60) {
			minutes += aMinutes;
		} else {
			minutes = (curMinutes + aMinutes) - 60;
			if (hours < 23) {
				++hours;
			} else {
				hours = 0;
			}
		}
	}
};

#endif