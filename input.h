/*
 * input.h
 *
 *  Created on: Mar 7, 2017
 *      Author: mt
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <iostream>

using namespace std;

int simple_hash(char* num, int len);
int execute_input(FILE* input, int max_args, int max_len, Hash_table** ht1, Hash_table** ht2/*,Heap* hp*/);
int execute_command(char ** words,Hash_table** ht1, Hash_table** ht2/*,Heap* hp*/);

#endif /* INPUT_H_ */
