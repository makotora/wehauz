/*
 * utilities.h
 *
 *  Created on: Mar 16, 2017
 *      Author: mt
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

class PathNode
{
	public:
		char direction;
		PathNode* next;

		PathNode(char dir);
		~PathNode();
};

class PathStack
{
	public:
		PathNode* first;

		PathStack();
		~PathStack();
		void push(char dir);
		char pop();
		void print();
};

#endif /* UTILITIES_H_ */
