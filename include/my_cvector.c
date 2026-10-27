#include "my_cvector.h"
#include <stdlib.h>

void vector_init(doublevector *V, int capacity)
{
	V->size = 0; //實際存入多少數字
	V->capacity = (capacity > 0) ? capacity : 2;
	V->data = (double *)calloc(V->capacity, sizeof(double));
	//(double*):指向double的指標
	//calloc:Clear Allocation（清零配置）
	//v->capacity:元素數量
	//sizeof(double):每個元素的大小
}

void vector_push_back(doublevector *V, double value)
{
	if (V->size == V->capacity) {
		V->capacity = (V->capacity == 0) ? 1 : 2 * V->capacity;
		V->data = (double *)realloc(V->data, V->capacity * sizeof(double));
	}
	V->data[V->size] = value;
	V->size++;
}

void vector_free(doublevector *V)
{
	free(V->data);
	V->data = NULL;
	V->capacity = 0;
	V->size = 0;
}
