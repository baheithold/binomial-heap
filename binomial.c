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

int getHEAPNODEkey(HEAPNODE *n) {
    assert(n != 0);
    return n->key;
}

void setHEAPNODEkey(HEAPNODE *n, int key) {
    assert(n != 0);
    n->key = key;
}

void *getHEAPNODEvalue(HEAPNODE *n) {
    assert(n != 0);
    return n->value;
}

void setHEAPNODEvalue(HEAPNODE *n, void *v) {
    assert(n != 0);
    n->value = v;
}

DLL *getHEAPNODEchildren(HEAPNODE *n) {
    assert(n != 0);
    return n->children;
}

void setHEAPNODEchildren(HEAPNODE *n, DLL *children) {
    assert(n != 0);
    n->children = children;
}

struct BINOMIAL {
    DLL *rootlist;
    HEAPNODE *extreme;
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
    rv->rootlist = newDLL(display, free);
    rv->extreme = NULL;
    rv->size = 0;
    rv->display = display;
    rv->compare = compare;
    rv->update = update;
    rv->free = free;
    return rv;
}

int sizeBINOMIAL(BINOMIAL *b) {
    assert(b != 0);
    return b->size;
}

void unionBINOMIAL(BINOMIAL *recipient, BINOMIAL *donor) {
    assert(recipient != 0);
    assert(donor != 0);
    unionDLL(recipient->rootlist, donor->rootlist);
    recipient->size += donor->size;
    donor->rootlist = newDLL(donor->display, donor->free);
    donor->size = 0;
    donor->extreme = NULL;
    // TODO: Consolidate recipient rootlist
}

void *peekBINOMIAL(BINOMIAL *b) {
    assert(b != 0);
    return getHEAPNODEvalue(b->extreme);
}

void statisticsBINOMIAL(BINOMIAL *b, FILE *fp) {
    assert(b != 0);
    fprintf(fp, "size: %d\nrootlist size: %d\n", b->size, sizeDLL(b->rootlist));
    if (b->size > 0) {
        fprintf(fp, "extreme: ");
        b->display(b->extreme, fp);
        fprintf(fp, "\n");
    }
}

void freeBINOMIAL(BINOMIAL *b) {
    assert(b != 0);
    freeDLL(b->rootlist);
    free(b);
}
