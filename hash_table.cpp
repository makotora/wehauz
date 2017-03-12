/*
 * hash_table.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: mt
 */

#include <iostream>
#include <cstring>
#include "hash_table.h"

using namespace std;

/*Class Bucket_entry*/
Bucket_entry::Bucket_entry(Number& num1, int& max_infos)
:number1(num1), infos_per_entry(max_infos)
{
	first_infos = new Call_infos(max_infos);
	last_infos = first_infos;
}


Bucket_entry::~Bucket_entry()
{
	if (first_infos != NULL)
		delete first_infos;/*Call infos destructor will recursively delete all next infos*/
}

void Bucket_entry::print()
{
	cout << "\t\t| "; number1.print(); cout << " |\n";;
	Call_infos* current = first_infos;
	int count = 0;

	while (current != NULL)
	{
		cout << "\t\t\t| Records node " << ++count << " |"<< endl;
		current->print();
		current = current->next;
	}
}
void Bucket_entry::print_range(Time* time1, Date* date1, Time* time2, Date* date2)
{
	cout << "\n\t\t| "; number1.print(); cout << " |\n\n";
	Call_infos* current = first_infos;

	while (current != NULL)
	{
		current->print_range(time1, date1, time2, date2);
		current = current->next;
	}
}


void Bucket_entry::add_info_node()
{
	Call_infos * new_infos = new Call_infos(infos_per_entry);
	new_infos->next = NULL;

	if (first_infos == NULL)
	{
		first_infos = new_infos;
		last_infos = new_infos;
	}
	else
	{
		last_infos->next = new_infos;
		last_infos = new_infos;
	}
}


void Bucket_entry::free_infos()
{
	if (first_infos != NULL)
		first_infos->free_infos();/*this will recursively free all next infos*/
}

int Bucket_entry::insert_info(Number& num2, CdrInfo* info)
{
	/*if there we are inserting the first info for this bucket*/
	/*or bucket's last node of infos is full*/
	if (first_infos == NULL || last_infos->count == infos_per_entry)
		add_info_node();/*add an info node (at end)*/

	return last_infos->add_info(num2, info);/*add info at last node,return count of infos at that node*/
}


int Bucket_entry::delete_info(char* cdrid, int free_info)
{
	int target_index = -1;

	Call_infos* current = first_infos;

	while (current != NULL)
	{
		target_index = current->find_info(cdrid);
		if (target_index != -1)
			break;

		current = current->next;
	}

	if (target_index == -1)
	{
		cout << "Delete: No call record with id '" << cdrid << "'\n";
		return -1;
	}

	Call_info** infos = current->infos;
	if (free_info == 1)
	{
		infos[target_index]->free_info();
	}

	delete infos[target_index];
	infos[target_index] = last_infos->infos[last_infos->count - 1];/*replace deleted info with last info*/
	last_infos->infos[last_infos->count - 1] = NULL;
	(last_infos->count)--;

	/*last infos node is now empty,delete it*/
	if (last_infos->count == 0)
	{
		if (first_infos != last_infos)
		{/*if we have at least 2 info nodes*/
			/*find previous node of last*/
			current = first_infos;
			while (current->next != last_infos)
			{
				current = current->next;
			}

			/*delete old last,and set its previous to be the new last*/
			delete last_infos;
			last_infos = current;
			current->next = NULL;/*this has no next now (we deleted it remember?)*/
		}
		else
		{/*only one info node,just delete it*/
			delete first_infos;
			first_infos = NULL;
			last_infos = NULL;
		}
	}

	return 0;

}



int Bucket_entry::communicated(Number number2)
{
	Call_infos* current_infos;
	int infos_count;

	current_infos = first_infos;

	while (current_infos != NULL)
	{
		infos_count = current_infos->count;

		for (int i = 0;i<infos_count;i++)
		{
			if (current_infos->infos[i]->number2 == number2)
				return 1;
		}
		current_infos = current_infos->next;
	}

	return 0;/*Didnt find any communication between num1 num2*/

}


/*Class Bucket*/
Bucket::Bucket(int& max_entries, int& max_infos)
:count(0), next(NULL), entries_per_bucket(max_entries), infos_per_entry(max_infos)
{
	entries = new Bucket_entry*[max_entries];
	int i;
	for (i=0;i<max_entries;i++)
		entries[i] = NULL;
}


Bucket::~Bucket()
{
	if (next != NULL)
		delete next;

	int i;
	for (i=0;i<count;i++)
		delete entries[i];

	delete[] entries;
}


void Bucket::print()
{
	int i;
	for (i=0;i<count;i++)
		entries[i]->print();
}


void Bucket::free_infos()
{
	int i;
	for (i=0;i<count;i++)
		entries[i]->free_infos();
}


int Bucket::find_entry(Number& num1)
{
	int i;
	for (i=0;i<count;i++)
	{
		if (entries[i]->number1 == num1)
			return i;
	}

	return -1;
}



/*Class Hash_table*/
Hash_table::Hash_table(uint32_t (*hashfun)(const char*,int) , int& max_bucks, int& bucket_size, int& max_infos)
:hash(hashfun), max_buckets(max_bucks), infos_per_entry(max_infos), total_buckets(0), collisions(0)
{
	entries_per_bucket = bucket_size / sizeof(Bucket_entry);
	buckets = new Bucket*[max_bucks];
	int i;
	for (i=0;i<max_bucks;i++)
		buckets[i] = NULL;
}


Hash_table::~Hash_table()
{
	int i;
	for (i=0;i<max_buckets;i++)
		delete buckets[i];/*this will recursively delete all next buckets in this list*/

	delete[] buckets;
}


void Hash_table::print()
{
	int i;
	for (i=0;i<max_buckets;i++)
	{
		cout << "\n| Hash_table[" << i << "] |\n";

		Bucket* current = buckets[i];
		int count = 0;
		while (current != NULL)
		{
			cout << "\t| Bucket " << ++count << " |\n";
			current->print();
			current = current->next;
		}
	}
}

void Hash_table::print_range(char* hash_key, char* strnum1, Time* time1, Date* date1, Time* time2, Date* date2)
{
	uint32_t bucket_num = (*hash)(hash_key, strlen(hash_key)) % max_buckets;

	Bucket* current = buckets[bucket_num];
	int bucket_entry = -1;

	Number num1(strnum1);

	while (current != NULL)
	{
		bucket_entry = current->find_entry(num1);
		if (bucket_entry != -1)
			break;

		current = current->next;
	}

	if (bucket_entry != -1)
	{
		current->entries[bucket_entry]->print_range(time1, date1, time2, date2);
	}
	else
	{
		cout << "No call record for number: '"; num1.print(); cout << "'\n";
	}
}


void Hash_table::free_infos()
{
	int i;
	for 	(i=0;i<max_buckets;i++)
	{
		Bucket* current = buckets[i];

		while (current != NULL)
		{
			current->free_infos();
			current = current->next;
		}
	}
}


int Hash_table::insert_info(char* hash_key, char* strnum1, char* strnum2, CdrInfo* info)
{
	uint32_t bucket_num = (*hash)(hash_key, strlen(hash_key)) % max_buckets;

	Bucket* current = buckets[bucket_num];
	/*for hash statistics*/
	if (current == NULL)
	{
		total_buckets++;
	}

	int bucket_entry = -1;

	Number num1(strnum1);
	Number num2(strnum2);

	if (current != NULL)
	{
		while (1)
		{
			bucket_entry = current->find_entry(num1);
			if (bucket_entry != -1 || current->next == NULL)
				break;

			current = current->next;
		}
	}

	Bucket* new_bucket;
	Bucket_entry* new_entry;

	if (bucket_entry != -1)
	{
//		cout << "Insert: A bucket entry for number "; num1.print(); cout << " exists! Inserting there\n";
		current->entries[bucket_entry]->insert_info(num2, info);
	}
	else
	{
		/*for hash statistics*/
		if (current != NULL)
		{
			collisions++;
		}
//		cout << "Insert: No bucket entry for number "; num1.print(); cout << ".Creating a new bucket entry!\n";
		/*SINCE WE SEARCHED TILL THE LAST BUCKET OF THE LIST,CURRENT IS THE LAST IN THE LIST*/

		if (current == NULL)/*there is not bucket in this hash table entry.Add one now*/
		{
			new_bucket = new Bucket(entries_per_bucket, infos_per_entry);
			buckets[bucket_num] = new_bucket;
			current = new_bucket;
		}
		else if (current->count == entries_per_bucket)
		{/*This bucket is full,add another*/
			new_bucket = new Bucket(entries_per_bucket, infos_per_entry);
			current->next = new_bucket;
			current = current->next;
		}

		new_entry = new Bucket_entry(num1, infos_per_entry);
		new_entry->insert_info(num2, info);
		current->entries[current->count] = new_entry;
		(current->count)++;
	}

	return 0;
}


int Hash_table::delete_info(char* hash_key, char* strnum1, char* cdrid, int free_info)
{
	uint32_t bucket_num = (*hash)(hash_key, strlen(hash_key)) % max_buckets;

	Bucket* first_bucket = buckets[bucket_num];
	Bucket* current_bucket = first_bucket;
	Bucket* last_bucket;
	Bucket_entry* current_entry;
	int bucket_entry = -1;

	Number num1(strnum1);

	if (current_bucket != NULL)
	{
		while (1)
		{
			bucket_entry = current_bucket->find_entry(num1);
			if (bucket_entry != -1 || current_bucket->next == NULL)
				break;

			current_bucket = current_bucket->next;
		}
	}

	if (bucket_entry != -1)
	{
		//cout << "Delete: Found bucket entry for number "; num1.print(); cout << ".Deleting!\n";
		current_entry = current_bucket->entries[bucket_entry];
		current_entry->delete_info(cdrid, free_info);

		if ( current_entry->isEmpty() )
		{
			//cout << "Bucket is now empty.Deleting bucket\n";
			delete current_entry;
			/*REPLACE THIS BUCKET'S DELETED ENTRY WITH THE LAST ENTRY IN THE LIST OF BUCKETS (delete bucket if last got empty)*/

			/*find last bucket*/
			last_bucket = current_bucket;/*I could also start from first_bucket but lets (maybe) save some time..Continue from where we left off*/
			while (last_bucket->next != NULL)
				last_bucket = last_bucket->next;

			current_entry = last_bucket->entries[last_bucket->count - 1];
			last_bucket->entries[last_bucket->count - 1] = NULL;
			(last_bucket->count)--;

			/*If last bucket is empty.Delete it*/
			if (last_bucket->count == 0)
			{
				if (first_bucket == last_bucket)/*if first bucket is last bucket.we are deleting the only bucket entry in this bucket*/
				{/*so delete this (now empty) bucket and note that hash_table has no buckets anymore*/
					delete first_bucket;
					//cout << "deleting first bucket!\n";
					buckets[bucket_num] = NULL;/*Make it NULL for the hash_table*/
				}
				else
				{/*make previous bucket of last have no next.delete last_bucket*/
					/*find previous bucket of last*/
					current_bucket = first_bucket;
					while (current_bucket->next != last_bucket)
						current_bucket = current_bucket->next;
					//cout << "deleting last bucket!\n";
					current_bucket->next = NULL;
					delete last_bucket;
				}
			}

		}

		return 0;
	}
	else
	{
		//cout << "Delete: No bucket entry for number "; num1.print(); cout << " !Nothing to delete.\n";
		return 1;
	}
}


Bucket_entry* Hash_table::get_entry(char* hash_key, char* strnum1)
{
	uint32_t bucket_num = (*hash)(hash_key, strlen(hash_key)) % max_buckets;

	Bucket* current_bucket = buckets[bucket_num];
	Number num1(strnum1);
	int bucket_entry = -1;

	if (current_bucket != NULL)
	{
		while (1)
		{
			bucket_entry = current_bucket->find_entry(num1);
			if (bucket_entry != -1 || current_bucket->next == NULL)
				break;

			current_bucket = current_bucket->next;
		}
	}

	if (bucket_entry != -1)
		return current_bucket->entries[bucket_entry];
	else
		return NULL;

}


void Hash_table::print_hash_stats()
{
	cout << "---Printing hash table statistics---\n";
	cout << "Hash table usage: " << ((float)(total_buckets) / (float)(max_buckets)) * 100 << "%\n";
	cout << "Number of collisions: " << collisions << endl;
}
