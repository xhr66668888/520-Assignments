#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include <stddef.h>

int running_total(int val);
int* reverse(int* arr, int len);
void reverse_in_place(int* arr, int len);
int num_occurences(int* arr, int len, int val);
int* remove_duplicates(int* arr, int len, int* new_len);
char* string_reverse(const char* str);
int* transpose(int* matrix, int rows, int cols);
char** split_string(const char* str, char delimiter, int* count);
void free_string_array(char** arr, int count);

#endif