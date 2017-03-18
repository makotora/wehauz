/*
 * heap.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: mt
 */
#include <iostream>
#include <iomanip>
#include "heap.h"

using namespace std;

/*class HeapNode*/
HeapNode::HeapNode(void* dat, float s)
: data(dat), sum(s), parent(NULL), left(NULL), right(NULL){}

HeapNode::HeapNode(void* dat, float s, HeapNode* par)
: data(dat), sum(s), parent(par), left(NULL), right(NULL){}

HeapNode::~HeapNode()
{
	if (left != NULL)
		delete left;
	if (right != NULL)
		delete right;
}

void HeapNode::print(void (*print_fun)(void* data), int level)
{
	for (int i=0;i<level;i++)
		cout << "\t";

	(*print_fun)(data);
	cout << ": " << fixed << setprecision(2) << sum << endl;

	if (left != NULL)
	{
		left->print(print_fun, level+1);
	}
	if (right != NULL)
	{
		right->print(print_fun, level+1);
	}
}


void HeapNode::delete_chaptrs()
{
	if (left != NULL)
		left->delete_chaptrs();
	if (right != NULL)
			right->delete_chaptrs();

	char* chrptr = (char*) data;
	delete[] chrptr;
}


/*class MaxHeap*/
MaxHeap::MaxHeap(int (*data_cmp)(void*,void*), void (*print_f)(void*))
: root(NULL), count(0), income(0), data_comp(data_cmp), print_fun(print_f){}


MaxHeap::~MaxHeap()
{
	if ( root != NULL )
		delete root;/*recursively delete all the heap starting from root (check HeapNode destructor)*/
}


HeapNode* MaxHeap::push(void* dat, float sum)
{
	count++;
	income += sum;
	PathStack* pathStack;
	char direction;
	HeapNode* parent;
	HeapNode* new_node;

	if (count == 1)/*we added root*/
	{
		root = new HeapNode(dat, sum);
		return root;
	}
	else
	{
		pathStack = find_path_to(count/2);/*find parent of the node 'count'*/
		/*(the node we are about to insert*/
		parent = root;
		direction = pathStack->pop();
		while ( direction != 'x' )
		{
			if ( direction == 'l')
			{
				parent = parent->left;
			}
			else
			{
				parent = parent->right;
			}

			direction = pathStack->pop();
		}

		new_node = new HeapNode(dat, sum, parent);

		if (count % 2 == 0)
		{
			parent->left = new_node;
		}
		else
		{
			parent->right = new_node;
		}

		delete pathStack;

		Heapify(new_node);

		return new_node;
	}
}


HeapNode* MaxHeap::pop()
{
	HeapNode* old_root = root;
	HeapNode* curr_node;
	HeapNode* last_node;
	char direction;

	PathStack* pathStack;

	if (count == 0)
		return NULL;
	else if (count == 1)
	{
		old_root = root;
		root = NULL;
		count = 0;
		return old_root;
	}

	pathStack = find_path_to(count/2);/*find parent of the node 'count'*/
	/*(the node we are about to insert*/
	curr_node = root;
	direction = pathStack->pop();
	while ( direction != 'x' )
	{
		if ( direction == 'l')
		{
			curr_node = curr_node->left;
		}
		else
		{
			curr_node = curr_node->right;
		}

		direction = pathStack->pop();
	}

	if (count % 2 == 0)
	{
		last_node = curr_node->left;
		curr_node->left = NULL;/*"delete it"*/
	}
	else
	{
		last_node = curr_node->right;
		curr_node->right = NULL;/*"delete it"*/
	}

	/*"swap" last node with root.(dont keep the root on the heap)*/
	last_node->left = root->left;
	last_node->right = root->right;
	last_node->parent = NULL;

	if (last_node->left != NULL)
		last_node->left->parent = last_node;
	if (last_node->right != NULL)
			last_node->right->parent = last_node;
	root = last_node;
	count--;

	delete pathStack;

	/*Heapify from root to bottom*/
	curr_node = root;
	while (curr_node->left != NULL || curr_node->right != NULL)
	{
		HeapNode* left_child = curr_node->left;
		HeapNode* right_child = curr_node->right;
		HeapNode* max_child = NULL;

		if (left_child == NULL)
		{
			max_child = right_child;
		}
		else if (right_child == NULL)
		{
			max_child = left_child;
		}
		else
		{
			if (left_child->sum > right_child->sum)
			{
				max_child = left_child;
			}
			else
			{
				max_child = right_child;
			}
		}

		if (max_child->sum > curr_node->sum)
			swap_nodes(curr_node, max_child);
		else
			break;/*heap is ok.Done*/
	}

	old_root->left = NULL;
	old_root->right = NULL;

	return old_root;
}

HeapNode* MaxHeap::find_node(void* target)
{
	return find_node_rec(root, target);
}


HeapNode* MaxHeap::find_node_rec(HeapNode* node, void* target)
{
	if (node == NULL)
		return NULL;

	HeapNode* left_ret;
	HeapNode* right_ret;
	//cout << "find_node_rec: ";number->print();cout << endl;

	if ( (*data_comp)(target, node->data))
		return node;

	left_ret = find_node_rec(node->left, target);
	if (left_ret != NULL)
		return left_ret;
	right_ret = find_node_rec(node->right, target);
	if (right_ret != NULL)
		return right_ret;

	return NULL;

}


void MaxHeap::Heapify(HeapNode* node)
{
//	print();
	while (node->parent != NULL)
	{
		if (node->parent->sum >= node->sum)/*if parent has a >= value than node*/
		{/*heapify is complete.*/
			break;
		}
		swap_nodes(node->parent, node);/*swap the nodes in the heap,and make the changes needed for the heap o still be intact*/
//		cout << "Heapify moved up" << node->sum << endl;
//		print();
	}

}


void MaxHeap::update(HeapNode* node, float add)
{
	income += add;
	node->sum += add;
	Heapify(node);
}


PathStack* MaxHeap::find_path_to(int node_num)
{
	PathStack* pathStack = new PathStack();

	while (node_num > 1)
	{
		if (node_num % 2 == 0)
			pathStack->push('l');
		else
			pathStack->push('r');

		node_num /= 2;
	}

	return pathStack;
}


void MaxHeap::print()
{
	cout << "Printing heap.Total income is: " << income << endl;
	if (root != NULL)
		root->print(print_fun, 0);
}


void MaxHeap::swap_nodes(HeapNode* parent, HeapNode* child)
{
	HeapNode* grandpa = parent->parent;
	HeapNode* tempLeft = child->left;
	HeapNode* tempRight = child->right;

	/*adjust parents*/
	child->parent = grandpa;
	parent->parent = child;

	/*fix/switch children values*/
	if (parent->left == child)
	{
		child->left = parent;
		child->right = parent->right;
	}
	else
	{
		child->left = parent->left;
		child->right = parent;
	}

	parent->left = tempLeft;
	parent->right = tempRight;

	/*parent's and child's childr	en (if they exist) must change parent*/
	/*basically the nodes will look at their childen and say "Hi, i am your new daddy" makes sense*/
	if (parent->left != NULL)
		parent->left->parent = parent;
	if (parent->right != NULL)
		parent->right->parent = parent;

	if (child->left != NULL)
		child->left->parent = child;
	if (child->right != NULL)
		child->right->parent = child;

	if (grandpa != NULL)/*if we are not switching root*/
	{
		if (grandpa->left == parent)
		{
			grandpa->left = child;
		}
		else
		{
			grandpa->right = child;
		}
	}
	else
	{
		root = child;
	}

}


void MaxHeap::topk(float k)
{
	if (k > 100 || k < 0)
		cout << "Invalid k given.k must be more than 0 and less or equal to 100" << endl;
	if (root == NULL)
	{
		cout << "Topk: Heap is empty!" << endl;
	}
	if (k == 0)/*0 percent means noone..*/
		return;

	MaxHeap heap;

	float target_income = k * income / (float) 100;
	cout << "target is " << target_income << endl;



	HeapNode* next_max = root;
	HeapNode* next_node;
	float next_sum = root->sum;
	float current_income = next_sum;
	Number* next_num = (Number*) next_max->data;
	/*print roots percent*/
	next_num->print();cout << " " << next_sum / income * 100 << "% (value: " << next_sum << ")" << endl;


	while (current_income < target_income)
	{
		if (next_max == NULL)/*Heap is now empty,nothing more to print*/
		{
			cout << "Topk: Heap is empty!" << endl;
			return;
		}

		if (next_max->left != NULL)
			heap.push(next_max->left, next_max->left->sum);

		if (next_max->right != NULL)
			heap.push(next_max->right, next_max->right->sum);

		next_node = heap.pop();/*pop the HeapNode.This heap has some of the original HeapNodes as data*/
		next_sum = 	next_node->sum;
		next_max = (HeapNode*) next_node->data;/*get the data of the HeapNode.which is another heapnode (a bit strange)*/

		next_num = (Number*) next_max->data;/*finally get the number which is data for the original heapnodes*/
		next_num->print();cout << " " << next_sum / income * 100 << "% (sum: " << next_sum << ")" << endl;

		delete next_node;
		current_income += next_sum;
	}

}


void MaxHeap::delete_datas()
{
	if (root != NULL)
		root->delete_chaptrs();
}

/*data compare functions*/
/*return 0 if data are equal,0 otherwise*/
int number_compare(void* data1, void* data2)
{
	Number* num1 = (Number*) data1;
	Number* num2 = (Number*) data2;

	return (num1 == num2);
}


int string_compare(void* data1, void* data2)
{
	char* str1 = (char*) data1;
	char* str2 = (char*) data2;

	return (strcmp(str1, str2) == 0);
}


void number_print(void* data)
{
	Number* number = (Number*) data;
	number->print();
}


void string_print(void* data)
{
	char* str = (char*) data;
	cout << str;
}
