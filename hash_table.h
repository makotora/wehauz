/*
 * hash_table.h
 *
 *  Created on: Mar 2, 2017
 *      Author: mt
 */

#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "cdr_info.h"
#include "superfasthash.h"
#include "heap.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class Call_info
{
	public:

		Number number2;
		CdrInfo* infoPtr;

		Call_info(Number& num2, CdrInfo* info);

		void print();
		void free_info();
};


class Call_infos
{
	public:

		Call_info** infos;
		Call_infos* next;
		int count;

		Call_infos(int& max_infos);
		~Call_infos();

		void print();
		void print_range(Time* time1, Date* date1, Time* time2, Date* date2);
		void free_infos();

		int add_info(Number num2, CdrInfo* info);
		int find_info(char* cdrid);

};


class Bucket_entry
{
	public:
		Number* number1;
		HeapNode* heap_node;
		Call_infos * first_infos;
		Call_infos * last_infos;
		int infos_per_entry;

		Bucket_entry(char* strnum1, int& max_infos);
		~Bucket_entry();

		void print();
		void print_range(Time* time1, Date* date1, Time* time2, Date* date2);
		void add_info_node();
		void free_infos();

		int insert_info(Number& num2, CdrInfo* info);
		int delete_info(char* cdrid, int free_info);
		bool isEmpty() { return first_infos == NULL; };

		void setHeapNode(HeapNode * node);
		HeapNode* getHeapNode();

		int communicated(Number number2);

};


class Bucket
{
	public:
		Bucket_entry** entries;
		int count;
		Bucket* next;

		int entries_per_bucket;
		int infos_per_entry;

		Bucket(int& max_entries, int& max_infos);
		~Bucket();

		void print();
		void free_infos();

		int find_entry(Number& num1);
};

class Hash_table
{
	public:
		Bucket** buckets;
		uint32_t (*hash)(const char *data, int len);/*pointer to hash_function used*/
		int max_buckets;
		int entries_per_bucket;
		int infos_per_entry;
		int total_buckets;
		int collisions;

		Hash_table(uint32_t (*hashfun)(const char*,int) , int& max_bucks, int& bucket_size, int& max_infos);
		~Hash_table();

		void print();
		void print_range(char* hash_key, char* num1, Time* time1, Date* date1, Time* time2, Date* date2);
		void free_infos();

		int insert_info(char* hash_key, char* num1, char* num2, CdrInfo* info);
		int delete_info(char* hash_key, char* num1, char* cdrid, int free_info);

		Bucket_entry* get_entry(char* hash_key, char* strnum1);

		void print_hash_stats();

};

#endif /* HASH_TABLE_H_ */
