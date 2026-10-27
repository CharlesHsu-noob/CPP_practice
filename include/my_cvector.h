#ifndef MY_CVECTOR_H
#define MY_CVECTOR_H

#include <stdlib.h>
typedef struct {
	int size;
	int capacity;
	double *data;
} doublevector;

void vector_init(doublevector *V, int capacity);
void vector_push_back(doublevector *V, double value);
void vector_free(doublevector *V);

#endif
