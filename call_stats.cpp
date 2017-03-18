/*
 * call_stats.cpp
 *
 *  Created on: Mar 11, 2017
 *      Author: mt
 */

#include "call_stats.h"
#include "heap.h"

CallNode::CallNode(Number& num, Bucket_entry* in)
: number(num), in_calls(in), isolated(1), next(NULL){}


CallNode::~CallNode()
{
	if (next != NULL)
		delete next;
}


void CallNode::delete_next()
{
	CallNode* target;

	target = next;
	next = target->next;

	target->next = NULL;
	delete target;
}



CallGraph::CallGraph()
: first(NULL), last(NULL){}


CallGraph::~CallGraph()
{
	delete first;/*this will recursively delete all CallNodes*/
}


void CallGraph::insert(Number& number, Bucket_entry* in)
{
	CallNode* newNode = new CallNode(number, in);

	if (first == NULL)
	{
		first = newNode;
		last = first;
	}
	else
	{
		last->next = newNode;
		last = newNode;
	}
}


int CallGraph::contains(Number& number)
{
	CallNode* current_node = first;

	while (current_node != NULL)
	{
		if (current_node->number == number)
			return 1;

		current_node = current_node->next;
	}

	return -1;
}


void CallGraph::find_isolated()
{
	if (first == NULL)
		return;/*Graph is empty,nothing to remove*/

	CallNode* checkNode;
	CallNode* otherNode;

	checkNode = first;

	while (checkNode != NULL)
	{
		Number& checkNumber = checkNode->number;
		otherNode = checkNode->next;

		while (otherNode != NULL)
		{
			Number& otherNumber = otherNode->number;

			if ( (checkNode->in_calls != NULL && checkNode->in_calls->communicated(otherNumber)) ||
					(otherNode->in_calls != NULL && otherNode->in_calls->communicated(checkNumber)) )
			{
				checkNode->isolated = 0;
				otherNode->isolated = 0;
			}

			otherNode = otherNode->next;
		}

		checkNode = checkNode->next;
	}
}



void CallGraph::print()
{
	CallNode* current_node = first;

	if (first == NULL)
	{
		cout << "No such customers found!\n";
	}

	while (current_node != NULL)
	{
		current_node->number.print();cout << endl;
		current_node = current_node->next;
	}
}



void CallGraph::print_isolated()
{
	CallNode* current_node = first;

	if (first == NULL)
	{
		cout << "No such customers found!\n";
	}

	while (current_node != NULL)
	{
		if (current_node->isolated == 1)
		{
			current_node->number.print();
			cout << endl;
		}

		current_node = current_node->next;
	}
}



void indist(char* caller1, char* caller2, Hash_table* ht1, Hash_table* ht2)
{
	Number call1(caller1);
	Number call2(caller2);

	Bucket_entry* caller1_out = ht1->get_entry(caller1, caller1);
	Bucket_entry* caller1_in = ht2->get_entry(caller1, caller1);

	Bucket_entry* caller2_out = ht1->get_entry(caller2, caller2);
	Bucket_entry* caller2_in = ht2->get_entry(caller2, caller2);

	CallGraph* graph = new CallGraph();

	/*check if those who communicated with caller1 have also communicated with caller2*/
	Call_infos* current_infos;
	int infos_count;
	Call_info** infos;

	Bucket_entry* in_entry;

	char* numstring;
	/*first check those who received a call from caller1*/
	current_infos = caller1_out->first_infos;


	while (current_infos != NULL)
	{
		infos_count = current_infos->count;
		infos = current_infos->infos;

		for (int i = 0;i<infos_count;i++)
		{
			Number& checkNumber = infos[i]->number2;

			if (checkNumber != call1 && checkNumber != call2 && graph->contains(checkNumber) == -1)
			{/*if its not already in our graph*/
				/*If caller2 called or was called by this number*/
				if ( (caller2_out != NULL && caller2_out->communicated(checkNumber)) || (caller2_in != NULL && caller2_in->communicated(checkNumber)) )
				{/*this number has communicated with both caller1 and caller2*/
					numstring = checkNumber.getString();
					in_entry = ht2->get_entry(numstring, numstring);
					graph->insert(checkNumber, in_entry);

					delete[] numstring;
				}
			}
		}

		current_infos = current_infos->next;
	}

	/*Next, check those who called caller1*/
	current_infos = caller1_in->first_infos;

	while (current_infos != NULL)
	{
		infos_count = current_infos->count;
		infos = current_infos->infos;

		for (int i = 0;i<infos_count;i++)
		{
			Number& checkNumber = infos[i]->number2;

			if (checkNumber != call1 && checkNumber != call2 && graph->contains(checkNumber) == -1)
			{/*if its not already in our graph*/
				/*If caller2 called or was called by this number*/
				if ( (caller2_out != NULL && caller2_out->communicated(checkNumber)) || (caller2_in != NULL && caller2_in->communicated(checkNumber)) )
				{/*this number has communicated with both caller1 and caller2*/
					numstring = checkNumber.getString();
					in_entry = ht2->get_entry(numstring, numstring);
					graph->insert(checkNumber, in_entry);

					delete[] numstring;
				}
			}
		}

		current_infos = current_infos->next;
	}

//	cout << "Printing graph\n";
//	graph->print();
//	cout << "Now using remove communications...\n";
	graph->find_isolated();
	graph->print_isolated();
	cout << endl;

	delete graph;
}



void topdest(char* caller, Hash_table* ht1)
{
	Number caller_num(caller);

	Bucket_entry* entry = ht1->get_entry(caller, caller);

	if (entry == NULL)
	{
		cout << "No calls found\n";
		return;
	}

	MaxHeap heap(string_compare, string_print);
	HeapNode* node;
	Call_infos* current_infos = entry->first_infos;
	Call_info** infos;
	int count;
	int max_calls;
	int num_calls;
	char* area;

	while (current_infos != NULL)
	{
		count = current_infos->count;
		infos = current_infos->infos;
		for (int i=0;i<count;i++)
		{
			Number& checkNumber = infos[i]->number2;
			node = heap.find_node(checkNumber.area);

			if (node == NULL)/*a node for this area doesnt exist in the heap yet*/
			{
				area = new char[4];
				strcpy(area, checkNumber.area);
				heap.push(area, 1);/*initialise its counter to one*/
			}
			else
			{
				heap.update(node, 1);/*add one to the counter for that country*/
			}
		}

		current_infos = current_infos->next;
	}

	node = heap.pop();/*there is at least one node.we checked at the beginning*/
	max_calls = node->sum;
	area = (char*) node->data;
	cout << "Country code: " << area << ", Calls made: " << max_calls << endl;
	delete[] area;
	delete node;

	node = heap.pop();
	if (node != NULL)
	{
		num_calls = node->sum;
		area = (char*) node->data;
	}
	else
	{
		return;
	}

	while (num_calls == max_calls)
	{
		cout << "Country code: " << area << ", Calls made: " << max_calls << endl;
		delete[] area;
		delete node;
		node = heap.pop();

		if (node != NULL)
		{
			num_calls = node->sum;
			area = (char*) node->data;
		}
		else
		{
			return;
		}
	}

	delete[] area;
	delete node;

	heap.delete_datas();/*delete all remaining data in the heap (destructor wont do it)*/
}


