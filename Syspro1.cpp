//============================================================================
// Name        : Syspro1.cpp
// Author      : mt
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <istream>
#include <cstring>
#include <cstdio>
#include <ctime>

#include "cdr_info.h"
#include "hash_table.h"
#include "input.h"

using namespace std;

int main()
{
	int max_buckets1 = 5;
	int max_buckets2 = 5;
	int bucket_size = 100;
	int max_infos = 2;

	Hash_table* ht1 = new Hash_table(&simple_hash, max_buckets1, bucket_size, max_infos);
	Hash_table* ht2 = new Hash_table(&simple_hash, max_buckets2, bucket_size, max_infos);
	/*Heap* hp;*/

	int start = time(NULL);
	int commands_read = execute_input(stdin, 6, 100, &ht1, &ht2);
	cout << "Commands count: " << commands_read << endl;
	cout << "Time elapsed: " << time(NULL) - start << endl;
	return 0;
}
