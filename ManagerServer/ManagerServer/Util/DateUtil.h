#pragma once
#include <string>
using namespace std;

class DateUtil
{
public:
	static string Now();

	static string CustomDatetime(struct Datetime& tm);

private:
	DateUtil();
};

struct Datetime
{
public:
	Datetime(int year = 1900, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0)
	{
		this->year = year;
		this->month = month;
		this->day = day;
		this->hour = hour;
		this->minute = minute;
		this->second = second;
	}

	int GetYear() { return year; }
	int GetMonth() { return month; }
	int GetDay() { return day; }
	int GetHour() { return hour; }
	int GetMinute() { return minute; }
	int GetSecond() { return second; }

private:
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

