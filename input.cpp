/*
 * input.cpp
 *
 *  Created on: Mar 7, 2017
 *      Author: mt
 */
#include <iostream>
#include <cstring>
#include <cstdio>

#include "hash_table.h"
#include "cdr_info.h"
#include "input.h"
#include "call_stats.h"
#include "superfasthash.h"

using namespace std;

uint32_t simple_hash(const char* num, int len)
{
	char area[4];
	strncpy(area, num, 3);

	return atoi(area);
}


int execute_input(FILE* input, int max_args, int max_len, Hash_table** ht1, Hash_table** ht2/*,Heap* hp*/)
{
	char* line = new char[max_args*20];
	char** words = new char*[max_args];
	int commands_count = 0;

	while (1)
	{
		if (fgets(line,100, input) == NULL)
			break;

		/*ignore blank lines*/
		if (strlen(line) == 0)
			continue;

		commands_count++;

		/*print the line you just read*/
		cout << commands_count << ": " << line << "\n";

		/*split line to words*/
		words[0] = strtok(line," \t\n");
		int index = 0;

		do
		{
			index++;
			words[index] = strtok(NULL," \t\n");
		}
		while(words[index] != NULL);

		/*Debug.print words we read/split*/
//		for (int i=0;i<index;i++)
//			cout << words[i] << endl;

		if (execute_command(words, ht1, ht2) == 1)
			break;

	}

	delete[] words;
	delete[] line;

	return commands_count;
}


int execute_command(char ** words,Hash_table** ht1, Hash_table** ht2/*,Heap* hp*/)
{
	CdrInfo* info;
	char** record_fields;
	Time* time1 = NULL;
	Date* date1 = NULL;
	Time* time2 = NULL;
	Date* date2 = NULL;
	Time* tempt;
	Date* tempd;
	int arg_count;
	int max_buckets1;
	int max_buckets2;
	int bucket_size;
	int max_infos;

	if ( strcmp(words[0],"insert") == 0)
	{
		record_fields = new char*[10];
		record_fields[0] = strtok(words[1], ";");
		int index = 0;

		do
		{
			index++;
			record_fields[index] = strtok(NULL, " ;\n");
		}
		while (record_fields[index] != NULL);

		/*check if fields are ok*/
//		for (int i=0;i<index;i++)
//			cout << record_fields[i] << endl;

		info = new CdrInfo(record_fields[0], record_fields[1], record_fields[2], record_fields[3], record_fields[4],record_fields[5], record_fields[6], record_fields[7], record_fields[8]);

		(*ht1)->insert_info(record_fields[1], record_fields[1], record_fields[2], info);
		(*ht2)->insert_info(record_fields[2], record_fields[2], record_fields[1], info);

		/*heap stuff*/

		delete[] record_fields;
	}
	else if ( strcmp(words[0],"delete") == 0)
	{
		(*ht1)->delete_info(words[1], words[1], words[2], 0);
	}
	else if ( strcmp(words[0],"find") == 0)
	{
		arg_count = 2;
		while (words[arg_count] != NULL)
			arg_count++;

		if (arg_count == 4)
		{
			if (strlen(words[2]) == 5)/*if its time*/
			{
				time1 = new Time(words[2]);
				time2 = new Time(words[3]);

				if ((*time2) < (*time1))
				{
					tempt = time1;
					time1 = time2;
					time2 = tempt;
				}
			}
			else
			{
				date1 = new Date(words[2]);
				date2 = new Date(words[3]);

				if ((*date2) < (*date1))
				{
					tempd = date1;
					date1 = date2;
					date2 = tempd;
				}
			}
		}
		else if (arg_count == 6)
		{
			time1 = new Time(words[2]);
			date1 = new Date(words[3]);
			time2 = new Time(words[4]);
			date2 = new Date(words[5]);

			if ((*date2) < (*date1))
			{
				tempd = date1;
				date1 = date2;
				date2 = tempd;

				tempt = time1;
				time1 = time2;
				time2 = tempt;
			}
			else if ((*date1) == (*date2))
			{
				if ((*time2) < (*time1))
				{
					tempt = time1;
					time1 = time2;
					time2 = tempt;
				}
			}
		}
		else if (arg_count != 2)
		{
			cout << "find: Invalid arguments!\n";
			return -1;
		}

		(*ht1)->print_range(words[1], words[1], time1, date1, time2, date2);

		if ( time1 != NULL)
		{
			delete time1;
			delete time2;
		}

		if (date1 != NULL)
		{
			delete date1;
			delete date2;
		}

	}
	else if ( strcmp(words[0],"lookup") == 0)
	{
		arg_count = 2;
		while (words[arg_count] != NULL)
			arg_count++;

		if (arg_count == 4)
		{
			if (strlen(words[2]) == 5)/*if its time*/
			{
				time1 = new Time(words[2]);
				time2 = new Time(words[3]);
			}
			else
			{
				date1 = new Date(words[2]);
				date2 = new Date(words[3]);
			}
		}
		else if (arg_count == 6)
		{
			time1 = new Time(words[2]);
			date1 = new Date(words[3]);
			time2 = new Time(words[4]);
			date2 = new Date(words[5]);
		}
		else if (arg_count != 2)
		{
			cout << "find: Invalid arguments!\n";
			return -1;
		}

		(*ht2)->print_range(words[1], words[1], time1, date1, time2, date2);

		if ( time1 != NULL)
		{
			delete time1;
			delete time2;
		}

		if (date1 != NULL)
		{
			delete date1;
			delete date2;
		}

	}
	else if ( strcmp(words[0],"indist") == 0)
	{
		indist(words[1], words[2], *ht1, *ht2);
	}
	else if ( strcmp(words[0],"topdest") == 0)
	{


	}
	else if ( strcmp(words[0],"top") == 0)
	{


	}
	else if ( strcmp(words[0],"bye") == 0)
	{
		max_buckets1 = (*ht1)->max_buckets;
		max_buckets2 = (*ht2)->max_buckets;
		bucket_size = (*ht1)->entries_per_bucket * sizeof(Bucket_entry);
		max_infos = (*ht1)->infos_per_entry;

		cout << "Hash table 1 statistics:\n";
		(*ht1)->print_hash_stats();
		cout << "Hash table 2 statistics:\n";
		(*ht2)->print_hash_stats();

		(*ht2)->free_infos();
		delete *ht1;
		delete *ht2;
		/*delete hp*/
		*ht1 = new Hash_table(&hash, max_buckets1, bucket_size, max_infos);
		*ht2 = new Hash_table(&hash, max_buckets2, bucket_size, max_infos);
		/*create heap*/



	}
	else if ( strcmp(words[0],"print") == 0)
	{
		if ( strcmp(words[1], "hashtable1") == 0)
		{
			cout << "---Printing hashtable1---\n\n";
			(*ht1)->print();
		}
		else if ( strcmp(words[1], "hashtable2") == 0)
		{
			cout << "---Printing hashtable2---\n\n";
			(*ht2)->print();
		}
		else
		{
			cout << "Invalid print command!\n\n";
		}

	}
	else if ( strcmp(words[0],"dump") == 0)
	{


	}
	else if ( strcmp(words[0],"exit") == 0 || strcmp(words[0],"exit()") == 0)
	{
		cout << "Hash table 1 statistics:\n";
		(*ht1)->print_hash_stats();
		cout << "Hash table 2 statistics:\n";
		(*ht2)->print_hash_stats();

		(*ht2)->free_infos();
		delete *ht1;
		delete *ht2;
		/*delete hp*/

		return 1;

	}


	return 0;
}

