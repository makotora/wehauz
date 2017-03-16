/*
 * utilities.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: mt
 */

#include <iostream>
#include "heap.h"
#include "utilities.h"


PathNode::PathNode(char dir)
: direction(dir), next(NULL){}

PathNode::~PathNode()
{
	if (next != NULL)
		delete next;
}


PathStack::PathStack()
: first(NULL) {}


PathStack::~PathStack()
{
	if (first != NULL)
		delete first;/*recursively delete all the stack*/
}


void PathStack::push(char dir)
{/*insert at start*/
	PathNode* new_node = new PathNode(dir);
	if (first == NULL)
	{
		first = new_node;
	}
	else
	{
		new_node->next = first;
		first = new_node;
	}
}


char PathStack::pop()
{
	char direction;

	/*remove/delete first*/
	if (first == NULL)
		return 'x';

	direction = first->direction;
	PathNode* target = first;
	first = first->next;

	target->next = NULL;
	delete target;

	return direction;
}


void PathStack::print()
{
	PathNode* current = first;

	while (current != NULL)
	{
		cout << current->direction << " ";
		current = current->next;
	}

	cout << endl;
}
