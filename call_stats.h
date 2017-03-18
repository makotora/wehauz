/*
 * call_stats.h
 *
 *  Created on: Mar 11, 2017
 *      Author: mt
 */

#ifndef CALL_STATS_H_
#define CALL_STATS_H_

#include <iostream>
#include "cdr_info.h"
#include "hash_table.h"

using namespace std;

class CallNode
{
	public:
		Number& number;
		Bucket_entry* in_calls;/*node's edges*/
		int isolated;
		CallNode* next;

		CallNode(Number& num, Bucket_entry* in);
		~CallNode();
		void delete_next();
};


class CallGraph
{
	public:
		CallNode* first;
		CallNode* last;

	CallGraph();
	~CallGraph();
	void insert(Number& number, Bucket_entry* in);
	int contains(Number& number);
	void find_isolated();
	void print();
	void print_isolated();
};

void indist(char* caller1, char* caller2, Hash_table* ht1, Hash_table* ht2);
void topdest(char* caller, Hash_table* ht1);

#endif /* CALL_STATS_H_ */
