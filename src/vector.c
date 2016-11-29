#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <string.h>


void
Vector_resize(Vector *v, int capacity)
{
    v->elements = realloc(v->elements, capacity * sizeof(void *));
    v->capacity = capacity;
}

Vector *
Vector_create(int capacity)
{
    Vector * v = (Vector *)malloc(sizeof(Vector));
    v->size = 0;    
    v->capacity = capacity;
    v->elements = calloc(v->capacity, v->capacity * sizeof(void *));
    return v;
}

int
Vector_size(Vector *v)
{
	return v->size;
}

void
Vector_add(Vector *v, void *element)
{
    if (v->size == v->capacity)
        Vector_resize(v, v->capacity * 2);

   	v->elements[v->size] = element;//strdup(element); 
    v->size++;
}

void *
Vector_get(Vector *v, int index)
{
    if (index < 0 || index >= v->size)
        return NULL;

    return v->elements[index];
}

int
Vector_getIndex(Vector *v, void *element)
{
	VIterator *it = Vector_getIterator(v);
	char *el;
	int index = -1;
	while ((el = (char *)VIterator_getNext(it)) != NULL) {
		if (strcmp(el, (char *)element)==0) {
			index = it->index - 1;
			break;
		}
	}
	VIterator_destroy(it);
	return index;	
}

void *
Vector_set(Vector *v, void *element, int index)
{
    if (index < 0 || index >= v->size)
        return NULL;

    void *tmp = v->elements[index];
    v->elements[index] = element;

    return tmp;
}

void *
Vector_remove(Vector *v, int index)
{
    if (index < 0 || index >= v->size)
        return NULL;

    void *tmp = v->elements[index];
	v->elements[index] = NULL;

    int i;
    for (i=index; i<v->size; i++) {
        v->elements[i] = v->elements[i+1];
    }

    v->size--;
    v->elements[v->size] = NULL;

    if (v->size > 0 && v->size == (v->capacity / 4)) {
        Vector_resize(v, (v->capacity / 2));
    }

    return tmp;
}

void
Vector_clear(Vector *v)
{
    int end = v->size-1;

    int start;

    for (start = end; start >=0; start--) {
       v->elements[start] = NULL; 
    }

    v->size = 0;
}

void
Vector_destroy(Vector *v)
{
    free(v->elements);
    free(v);
}

VIterator *
Vector_getIterator(Vector *v)
{
    VIterator *it = malloc(sizeof(VIterator));
    it->vector = v;
    it->index = 0;
    it->next = Vector_get(v, 0);
    return it;
}

void
VIterator_reset(VIterator *it)
{
	it->index = 0;
	it->next = Vector_get(it->vector, 0);
}

void *
VIterator_getNext(VIterator *it)
{
    void *ret = it->next;
    it->index++;
    it->next = Vector_get(it->vector, it->index);
    return ret;
}

void
VIterator_destroy(VIterator *it)
{
    free(it);
}

