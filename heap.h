/*
 * heap.h
 *
 *  Created on: Mar 14, 2017
 *      Author: mt
 */

#ifndef HEAP_H_
#define HEAP_H_

#include "cdr_info.h"
#include "utilities.h"


class HeapNode
{
	public:
		void* data;
		float sum;

		HeapNode* parent;
		HeapNode* left;
		HeapNode* right;

		HeapNode(void* dat, float sum);
		HeapNode(void* dat, float sum, HeapNode* parent);
		~HeapNode();
		void print(void (*print_fun)(void* data), int level);
		void delete_chaptrs();
};

class MaxHeap
{
	public:
		HeapNode* root;
		int count;
		float income;
		int (*data_comp)(void* data1 ,void* data2);
		void (*print_fun)(void* data);

		MaxHeap(int (*data_comp)(void*,void*) = NULL, void (*print_fun)(void*) = NULL);
		~MaxHeap();

		HeapNode* push(void* dat, float sum);
		HeapNode* pop();
		HeapNode* find_node(void* target);
		HeapNode* find_node_rec(HeapNode* node, void* target);
		void update(HeapNode* node, float add);
		PathStack* find_path_to(int node_num);
		void Heapify(HeapNode* node);
		void print();
		void swap_nodes(HeapNode* parent, HeapNode* child);
		void topk(float k);
		void delete_datas();
};


/*data compare functions*/
int number_compare(void* data1, void* data2);
int string_compare(void* data1, void* data2);

/*data print functions*/
void number_print(void* data);
void string_print(void* data);

#endif /* HEAP_H_ */
