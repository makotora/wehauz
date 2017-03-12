/*
 * info_list.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: mt
 */

#include <iostream>
#include <cstring>
#include "hash_table.h"

using namespace std;

/*Class Call_info*/
Call_info::Call_info(Number& num2, CdrInfo* info)
:number2(num2), infoPtr(info)
{}


void Call_info::print()
{
	//cout << "\t\t\t|"; number2.print(); cout << "|\n";
	infoPtr->print();
}


void Call_info::free_info()
{
	delete infoPtr;
}

/*Class Call_infos*/
Call_infos::Call_infos(int& max_infos)
:next(NULL), count(0)
{
	infos = new Call_info*[max_infos];
	int i;
	for (i=0;i<max_infos;i++)
		infos[i] = NULL;
}


Call_infos::~Call_infos()
{
	if (next != NULL)
		delete next;

	int i;
	for (i=0;i<count;i++)
		delete infos[i];

	delete[] infos;
}


void Call_infos::print()
{
	int i;
	for (i=0;i<count;i++)
		infos[i]->print();
}


void Call_infos::print_range(Time* time1, Date* date1, Time* time2, Date* date2)
{
	int i;
	for (i=0;i<count;i++)
	{
		if (infos[i]->infoPtr->isInRange(time1, date1, time2, date2))
			infos[i]->print();
	}

}


void Call_infos::free_infos()
{
	if (next != NULL)
	{
		next->free_infos();
	}

	int i;
	for (i=0;i<count;i++)
		infos[i]->free_info();
}


int Call_infos::add_info(Number num2, CdrInfo* info)
{
	infos[count] = new Call_info(num2, info);
	count++;

	return count;
}


int Call_infos::find_info(char* cdrid)
{
	int i;
	for (i=0;i<count;i++)
	{
		if ( !strcmp(cdrid, infos[i]->infoPtr->cdr_uniq_id) )
			return i;
	}

	return -1;
}
