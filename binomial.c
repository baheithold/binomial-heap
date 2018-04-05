/*
 *  File:   binomial.c
 *  Author: Brett Heithold
 *  Description:
 */

#include "binomial.h"
#include "dll.h"
#include <stdlib.h>
#include <assert.h>

typedef struct heapnode {
    int key;
    void *value;
    DLL *children;
    void (*display)(void *, FILE *);
    void (*free)(void *);
} HEAPNODE;

HEAPNODE *newHEAPNODE(void *v, void (*d)(void *, FILE *), void (*f)(void *)) {
    HEAPNODE *n = malloc(sizeof(HEAPNODE));
    n->key = 0;
    n->value = v;
    n->children = newDLL(d, f);
    n->display = d;
    n->free = f;
}

struct BINOMIAL {
    DLL *rootList;
    int size;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*update)(void *, void *);
    void (*free)(void *);
};

BINOMIAL *newBINOMIAL(
        void (*display)(void *, FILE *),
        int (*compare)(void *, void *),
        void (*update)(void *, void *),
        void (*free)(void *)) {
    BINOMIAL *rv = malloc(sizeof(BINOMIAL));
    assert(rv != 0);
    rv->rootList = newDLL(display, free);
    rv->size = 0;
    rv->display = display;
    rv->compare = compare;
    rv->update = update;
    rv->free = free;
    return rv;
}
