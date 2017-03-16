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
		void print(int level);
};

class MaxHeap
{
	public:
		HeapNode* root;
		int count;
		float income;

		MaxHeap();
		~MaxHeap();

		HeapNode* push(void* dat, float sum);
		HeapNode* pop();
		void update(HeapNode* node, float add);
		PathStack* find_path_to(int node_num);
		void Heapify(HeapNode* node);
		void print();
		void swap_nodes(HeapNode* parent, HeapNode* child);
		void topk(float k);
};




#endif /* HEAP_H_ */
