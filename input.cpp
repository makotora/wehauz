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


float* read_charges(FILE* input)
{
	float* charges = new float[5];
	for (int i=0;i<5;i++)
		charges[i] = -1;

	char* line = new char[100];
	char** words = new char*[4];
	int count = 0;

	while (1)
	{
		if (fgets(line,100, input) == NULL)
			break;

		/*ignore blank lines or lines that start with '#'*/
		if (strlen(line) == 0 || line[0] == '#')
			continue;

		/*print the line you just read*/
//		cout << line << "\n";

		if (count == 5)
		{
			delete[] charges;
			charges = NULL;
			cout << "Invalid charges file!Too many charges!(should be 5)" << endl;

			return NULL;
		}

		/*split line to words*/
		words[0] = strtok(line,";");
		int index = 0;

		do
		{
			index++;
			words[index] = strtok(NULL,";\n");
		}
		while(words[index] != NULL);

		if ( !strcmp(words[0],"0") )
		{
			if (charges[0] == -1)
				charges[0] = atof(words[2]);
			else
				cout << "Error.Multiple charges for type 0";
		}
		else
		{
			if ( !strcmp(words[0],"1") )
			{
				if ( !strcmp(words[1],"1") )
				{
					if (charges[1] == -1)
						charges[1] = atof(words[2]);
					else
						cout << "Error.Multiple charges for type 1,tarrif 1";
				}
				else if ( !strcmp(words[1],"2") )
				{
					if (charges[2] == -1)
						charges[2] = atof(words[2]);
					else
						cout << "Error.Multiple charges for type 1,tarrif 2";
				}
			}
			else if ( !strcmp(words[0],"2") )
			{
				if ( !strcmp(words[1],"1") )
				{
					if (charges[3] == -1)
						charges[3] = atof(words[2]);
					else
						cout << "Error.Multiple charges for type 2,tarrif 1";
				}
				else if ( !strcmp(words[1],"2") )
				{
					if (charges[4] == -1)
						charges[4] = atof(words[2]);
					else
						cout << "Error.Multiple charges for type 2,tarrif 2";
				}
			}
		}

		/*Debug.print words we read/split*/
//		for (int i=0;i<index;i++)
//			cout << words[i] << endl;

		count++;
	}

	delete[] words;
	delete[] line;

	if (count < 5)
	{
		delete[] charges;
		charges = NULL;
		cout << "Invalid charges file!Not enough charges!(should be 5)" << endl;
	}

//	for (int i=0;i<5;i++)
//		cout << charges[i] << endl;


	return charges;
}


int execute_input(FILE* input, int max_args, int max_len, Hash_table** ht1, Hash_table** ht2, MaxHeap** hp, float* charges)
{
	char* line = new char[max_len];
	char** words = new char*[max_args+1];
	int commands_count = 0;

	while (1)
	{
		if (fgets(line,max_len, input) == NULL)
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

		if (execute_command(words, ht1, ht2, hp, charges) == 1)
			break;

	}

	delete[] words;
	delete[] line;

	return commands_count;
}


int execute_command(char ** words,Hash_table** ht1, Hash_table** ht2, MaxHeap** hp, float* charges)
{
	CdrInfo* info;
	char** record_fields;
	Time* time1 = NULL;
	Date* date1 = NULL;
	Time* time2 = NULL;
	Date* date2 = NULL;
	Bucket_entry* entry;
	HeapNode* heapnode;
	int arg_count;
	int max_buckets1;
	int max_buckets2;
	int bucket_size;
	int max_infos;
	float sum = 0;

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

		if (info->fault_condition < 200 || info->fault_condition > 299)
		{/*If there was some error with the call,just insert into hash tables.No charges to add at heap*/
			(*ht1)->insert_info(record_fields[1], record_fields[1], record_fields[2], info);
			(*ht2)->insert_info(record_fields[2], record_fields[2], record_fields[1], info);
		}
		else
		{/*calculate the charge for that call (sum)*/
			if (info->type == 0)
			{
				sum = charges[0];
			}
			else if (info->type == 1)
			{
				if (info->tarrif == 1)
				{
					sum = (float) info->duration * charges[1];
				}
				else if (info->tarrif == 2)
				{
					sum = (float) info->duration * charges[2];
				}
			}
			else if (info->type == 2)
			{
				if (info->tarrif == 1)
				{
					sum = (float) info->duration * charges[3];
				}
				else if (info->tarrif == 2)
				{
					sum = (float) info->duration * charges[4];
				}
			}
			else
			{
				cout << "Unknown call type!No charge!\n";
			}

			/*check if an entry for this caller already exists*/
			entry = (*ht1)->get_entry(record_fields[1], record_fields[1]);
			if ( entry != NULL)/*if it does*/
			{
				heapnode = entry->getHeapNode();/*get the pointer to the callers heap node so we can update it*/
				if (heapnode == NULL)/*if its null,it means that entry exists BUT heapnode doesnt (because of error code,it wasnt charged*/
				{
					heapnode = (*hp)->push(entry->number1, sum);/*So push it now for the first time.no error code now*/
					entry->setHeapNode(heapnode);/*save the pointer to it for next time*/
				}
				else
				{
					(*hp)->update(heapnode, sum);
				}
				/*Now insert at the hashtable (if we did it before checking if its there..it would be there*/
				(*ht1)->insert_info(record_fields[1], record_fields[1], record_fields[2], info);
				(*ht2)->insert_info(record_fields[2], record_fields[2], record_fields[1], info);
			}
			else/*entry does not exist.push a new heapnode at heap for this (new) caller after inserting to hash table*/
			{
				(*ht1)->insert_info(record_fields[1], record_fields[1], record_fields[2], info);
				(*ht2)->insert_info(record_fields[2], record_fields[2], record_fields[1], info);
				entry = (*ht1)->get_entry(record_fields[1], record_fields[1]);
				heapnode = (*hp)->push(entry->number1, sum);/*points to the same number1 as the entry*/
				entry->setHeapNode(heapnode);/*save the pointer to the node for next inserts*/
			}
		}


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

			}
			else
			{
				date1 = new Date(words[2]);
				date2 = new Date(words[3]);

				if ((*date1) > (*date2) )
				{/*error.invalid range*/
					cout << "Invalid range! date1 must be before or the same as date2!" << endl;
					delete date1;
					delete date2;
					return -1;
				}
			}
		}
		else if (arg_count == 6)
		{
			if (strlen(words[2]) == 5)/*if time is first*/
			{
				time1 = new Time(words[2]);
				date1 = new Date(words[3]);
				time2 = new Time(words[4]);
				date2 = new Date(words[5]);
			}
			else/*date is first*/
			{
				date1 = new Date(words[2]);
				time1 = new Time(words[3]);
				date2 = new Date(words[4]);
				time2 = new Time(words[5]);
			}

			if ((*date1) > (*date2) )
			{/*error.invalid range*/
				cout << "Invalid range! date1 must be before or the same as date2!" << endl;
				delete date1;
				delete date2;
				delete time1;
				delete time2;
				return -1;
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

				if ((*date1) > (*date2) )
				{/*error.invalid range*/
					cout << "Invalid range! date1 must be before or the same as date2!" << endl;
					delete date1;
					delete date2;
					return -1;
				}
			}
		}
		else if (arg_count == 6)
		{
			time1 = new Time(words[2]);
			date1 = new Date(words[3]);
			time2 = new Time(words[4]);
			date2 = new Date(words[5]);

			if ((*date1) > (*date2) )
			{/*error.invalid range*/
				cout << "Invalid range! date1 must be before or the same as date2!" << endl;
				delete date1;
				delete date2;
				delete time1;
				delete time2;
				return -1;
			}

		}
		else if (arg_count != 2)
		{
			cout << "lookup: Invalid arguments!\n";
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
		topdest(words[1], *ht1);
	}
	else if ( strcmp(words[0],"top") == 0)
	{
		(*hp)->topk(atof(words[1]));
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
		delete *hp;
		*ht1 = new Hash_table(&hash, max_buckets1, bucket_size, max_infos);
		*ht2 = new Hash_table(&hash, max_buckets2, bucket_size, max_infos);
		*hp = new MaxHeap(number_compare, number_print);/*create a heap with Numbers*/
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
		delete *hp;

		return 1;
	}
	else
	{
		cout << "Unknown command!" << endl;
	}


	return 0;
}

