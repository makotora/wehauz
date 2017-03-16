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
	int max_buckets1 = 10;
	int max_buckets2 = 10;
	int bucket_size = 1000;
	int max_infos = 100;

	Hash_table* ht1 = new Hash_table(&hash, max_buckets1, bucket_size, max_infos);
	Hash_table* ht2 = new Hash_table(&hash, max_buckets2, bucket_size, max_infos);
	/*Heap* hp;*/

	int start = time(NULL);
	int commands_read = execute_input(stdin, 6, 100, &ht1, &ht2);
	cout << "Commands count: " << commands_read << endl;
	cout << "Time elapsed: " << time(NULL) - start << " seconds" << endl;

	PathStack* stack;
	MaxHeap* heap = new MaxHeap();
//	stack = heap->find_path_to(12);
//	stack->print();
//	delete stack;
//	stack = heap->find_path_to(15);
//	stack->print();
//	delete stack;
//	stack = heap->find_path_to(7);
//	stack->print();
//	delete stack;
	Number* num = new Number("030-1234567890");
	heap->push(num, 1);
	heap->push(num, 2);
	heap->push(num, 3);
	heap->push(num, 4);
	heap->push(num, 5);
	heap->push(num, 6);
	heap->print();

//	HeapNode* node;
//	node = heap->pop();
//	cout << "Popped; " << node->sum << endl;
//	delete node;
//	node = heap->pop();
//	cout << "Popped; " << node->sum << endl;
//	delete node;
//	node = heap->pop();
//	cout << "Popped; " << node->sum << endl;
//	delete node;
//	node = heap->pop();
//	cout << "Popped; " << node->sum << endl;
//	delete node;
//	node = heap->pop();
//	cout << "Popped; " << node->sum << endl;
//	delete node;
//	node = heap->pop();
//	cout << "Popped; " << node->sum << endl;
//	Number* popnum = (Number*) node->data;
//	popnum->print();
//	delete node;

	heap->topk(50);

//	heap->print();


	delete heap;
	delete num;

	return 0;
}
