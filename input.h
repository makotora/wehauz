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

uint32_t simple_hash(const char* num, int len);
float* read_charges(FILE* input);
int execute_input(FILE* input, int max_args, int max_len, Hash_table** ht1, Hash_table** ht2, MaxHeap** hp, float* charges);
int execute_command(char ** words,Hash_table** ht1, Hash_table** ht2, MaxHeap** hp, float* charges);

#endif /* INPUT_H_ */
