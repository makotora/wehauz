/*
 * data.h
 *
 *  Created on: Feb 26, 2017
 *      Author: mt
 */
#ifndef CDR_INFO_H
#define CDR_INFO_H

#include <iostream>
#include <cstring>

using namespace std;

class Number
{
	public:
		char area[4];
		char num[11];

		Number(char * number);
		void print();
		char* getString();

		bool operator == (const Number& number) const
		{ return !strcmp(area, number.area) && !strcmp(num, number.num); }

		bool operator != (const Number& number) const
		{ return strcmp(area, number.area) || strcmp(num, number.num); }
};



class Date
{
	public:
		int day;
		int month;
		int year;

		Date(char* date);
		void print();
		int Compare(const Date&  date) const;

		bool operator == (const Date&  date) const
		{	return !Compare(date); }

		bool operator < (const Date&  date) const
		{ return Compare(date) < 0; }

		bool operator > (const Date&  date) const
		{ return Compare(date) > 0; }

		bool operator <= (const Date&  date) const
		{ return Compare(date) <= 0; }

		bool operator >= (const Date&  date) const
		{ return Compare(date) >= 0; }
};


class Time
{
	public:
		int hour;
		int minute;

		Time(char* time);
		void print();
		int Compare(const Time& time) const;

		bool operator == (const Time& time) const
		{	return !Compare(time); }

		bool operator < (const Time&  time) const
		{ return Compare(time) < 0; }

		bool operator > (const Time&  time) const
		{ return Compare(time) > 0; }

		bool operator <= (const Time&  time) const
		{ return Compare(time) <= 0; }

		bool operator >= (const Time&  time) const
		{ return Compare(time) >= 0; }
};

class CdrInfo
{
	public:
		char* cdr_uniq_id;
		Number caller;
		Number callee;
		Date date;
		Time init_time;
		int duration;
		int type;
		int tarrif;
		int fault_condition;

		CdrInfo(char* id,char* call, char* calle, char* d, char* t, char* dur, char* typ, char* tar, char* f);
		~CdrInfo() { delete[] cdr_uniq_id; }
		void print();
		bool isInRange(Time* time1 = NULL, Date* date1 = NULL, Time* time2 = NULL, Date* date2 = NULL);

};


#endif /*CRD_INFO_H*/
