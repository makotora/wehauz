/*
 * data.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: mt
 */
#include "cdr_info.h"

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

/*Class Number*/
Number::Number(char* number)
{
	char* token;
	char* copy = new char[strlen(number)+1];
	strcpy(copy, number);

	if ( strlen(number) != 14 )
	{
		strcpy(area, "000");
		strcpy(area, "0000000000");
		cout << "Invalid  phone number\n";
		delete[] copy;
		return;
	}

	token = strtok(copy, "-");

	if ( strlen(token) != 3 )
	{
		strcpy(area, "000");
		strcpy(area, "0000000000");
		cout << "Invalid  phone number\n";
		delete[] copy;
		return;
	}
	else
	{
		strcpy(area, token);
	}

	token = strtok(NULL, "-");

	if ( strlen(token) != 10)
	{
		cout << "Invalid  phone number\n";
		return;
	}
	else
	{
		strcpy(num, token);
	}

	delete[] copy;
}


char* Number::getString()
{
	char* num_string = new char[15];
	char* start = num_string;

	strncpy(num_string, area, 3);
	num_string += 3;
	*num_string = '-';
	num_string++;
	strncpy(num_string, num, 10);

	num_string = start;
	num_string[14] = '\0';

	return num_string;
}


void Number::print()
{
	cout << area << '-' << num;
}


/*Class Date*/
Date::Date(char * date)
{
	if ( strlen(date) != 8 )
	{
		day = 0;
		month = 0;
		year = 0;
		cout << "Invalid date\n";
		return;
	}

	char daystr[3];
	char monthstr[3];
	char yearstr[5];

	strncpy(daystr, date, 2);
	date += 2;
	strncpy(monthstr, date, 2);
	date += 2;
	strncpy(yearstr, date, 4);

	day = atoi(daystr);
	month = atoi(monthstr);
	year = atoi(yearstr);

}


void Date::print()
{
	cout << day << '-' << month << '-' << year;
}


int Date::Compare(const Date& date) const
{
	if (year < date.year)
		return -1;
	else if (year > date.year)
		return 1;
	else if (month < date.month)
		return -1;
	else if (month > date.month)
		return 1;
	else if (day < date.day)
		return -1;
	else if (day > date.day)
		return 1;

	return 0;
}


/*Class Time*/
Time::Time(char* time)
{
	char* token;
	char hourstr[3];
	char minutestr[3];

	char* copy;
	copy = new char[strlen(time)+1];
	strcpy(copy, time);

	if ( strlen(time) != 5 )
	{
		cout << "Invalid  time\n";
		hour = 0;
		minute = 0;
		delete[] copy;
		return;
	}

	token = strtok(copy, ":");

	if ( strlen(token) != 2 )
	{
		cout << "Invalid  time\n";
		hour = 0;
		minute = 0;
		delete[] copy;
		return;
	}
	else
	{
		strcpy(hourstr, token);
	}

	token = strtok(NULL, "-");

	if ( strlen(token) != 2)
	{
		hour = 0;
		minute = 0;
		delete[] copy;
		cout << "Invalid  phone number\n";
		return;
	}
	else
	{
		strcpy(minutestr, token);
	}

	hour = atoi(hourstr);
	minute = atoi(minutestr);

	delete[] copy;
}


void Time::print()
{
	cout << hour << ':' << minute;
}


int Time::Compare(const Time& time) const
{
	if (hour < time.hour)
		return -1;
	else if (hour > time.hour)
		return 1;
	else if (minute < time.minute)
		return -1;
	else if (minute > time.minute)
		return 1;

	return 0;
}


/*Class CdrInfo*/
CdrInfo::CdrInfo(char* id,char* call, char* calle, char* d, char* t, char* dur, char* typ, char* tar, char* f)
: caller(call), callee(calle), date(d), init_time(t)
{
	cdr_uniq_id = new char[strlen(id)+1];
	strcpy(cdr_uniq_id, id);

	duration = atoi(dur);
	type = atoi(typ);
	tarrif = atoi(tar);
	fault_condition = atoi(f);

}


void CdrInfo::print()
{
	cout << "\t\t\t--- Call id: " << cdr_uniq_id << "---\n";
	cout << "\t\t\tCaller: "; caller.print(); cout << endl;
	cout << "\t\t\tCallee: "; callee.print(); cout << endl;
	cout << "\t\t\tDate: "; date.print(); cout << endl;
	cout << "\t\t\tInit Time: "; init_time.print(); cout << endl;
	cout << "\t\t\tDuration: " << duration << " minutes\n";
	cout << "\t\t\tType: " << type << endl;
	cout << "\t\t\tTarrif: " << tarrif << endl;
	cout << "\t\t\tFault condition: " << fault_condition << '\n' << endl;

}


bool CdrInfo::isInRange(Time* time1, Date* date1, Time* time2, Date* date2)
{
	/*error checking,both arguments for each type,must be given*/
	if (time1 == NULL || time2 == NULL)
	{
		if (time1 != time2)
		{
			cout << "isInRange: Invalid arguments!\n";
			return false;
		}
	}

	if (date1 == NULL || date2 == NULL)
	{
		if (date1 != date2)
		{
			cout << "isInRange: Invalid arguments!\n";
			return false;
		}
	}

	if (time1 == NULL && date1 == NULL)
	{
		return true;
	}
	else if (time1 == NULL)
	{
		return (*date1) <= date && date <= (*date2);
	}
	else if (date1 == NULL)
	{
		return (*time1) <= init_time && init_time <= (*time2);
	}
	else
	{
		/*if its between the dates its certainly in range*/
		if ( (*date1) < date && date < (*date2) )
		{
			return true;
		}
		/*if its date is one of the "limit" dates we have to check the times*/
		else if (date == *date1 || date == *date2)
		{
			/*range is on the same day*/
			if (date1 == date2)
			{
				return (*time1) <= init_time && init_time <= (*time2);
			}
			else if (date == *date1)
			{
				return (*time1) <= init_time;
			}
			else
			{
				return init_time <= (*time2);
			}
		}
		/*Its not inside the date range,no need to check time*/
		else
		{
			return false;
		}
	}
}

