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
#include "heap.h"
#include "input.h"
#include "superfasthash.h"

using namespace std;

int main()
{
	FILE* charges_file = NULL;
	FILE* operations_file = NULL;
	float* charges;

	operations_file = fopen("/home/mt/Desktop/DI/Syspro/prj1/myinput2.txt", "r");

	charges_file = fopen("charge-config.txt", "r");
	if (charges_file == NULL)
	{
		cout << "Error.Charges file does not exist!" << endl;
		exit(1);
	}
	charges = read_charges(charges_file);

	int max_buckets1 = 10;
	int max_buckets2 = 10;
	int bucket_size = 1000;
	int max_infos = 100;

	Hash_table* ht1 = new Hash_table(&hash, max_buckets1, bucket_size, max_infos);
	Hash_table* ht2 = new Hash_table(&hash, max_buckets2, bucket_size, max_infos);
	MaxHeap* hp = new MaxHeap(number_compare, number_print);/*create a heap with Numbers*/

	int start = time(NULL);
	int commands_read;
	commands_read = execute_input(operations_file, 6, 100, &ht1, &ht2, &hp, charges);
	commands_read = execute_input(stdin, 6, 100, &ht1, &ht2, &hp, charges);
	cout << "Commands count: " << commands_read << endl;
	cout << "Time elapsed: " << time(NULL) - start << " seconds" << endl;


	delete[] charges;
	fclose(charges_file);
/*REMEMBER TO CLOSE FILES AND FREE*/
	return 0;
}
