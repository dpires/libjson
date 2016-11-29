#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct Vector {
    int size;
    int capacity;
    void **elements;
} Vector;

Vector *
Vector_create(int capacity);

int
Vector_size(Vector *v);

void
Vector_add(Vector *v, void *element);

void *
Vector_get(Vector *v, int index);

int
Vector_getIndex(Vector *v, void *element);

void *
Vector_set(Vector *v, void *element, int index);

void *
Vector_remove(Vector *v, int index);

void
Vector_clear(Vector *v);

void
Vector_destroy(Vector *v);

void
Vector_resize(Vector *v, int capacity);

typedef struct VIterator {
    Vector *vector;
    int index;
    void *next;
} VIterator;

VIterator *
Vector_getIterator(Vector *v);

void
VIterator_reset(VIterator *it);

void *
VIterator_getNext(VIterator *it);

void
VIterator_destroy(VIterator *it);

#endif
