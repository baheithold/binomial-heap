/*
 *  File:   binomial.c
 *  Author: Brett Heithold
 *  Description:
 */

#include "binomial.h"
#include "dll.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct HEAPNODE HEAPNODE;

struct HEAPNODE {
    int key;
    void *value;
    HEAPNODE *parent;
    DLL *children;
    DLL *owner;
    void (*display)(void *, FILE *);
    void (*free)(void *);
};

HEAPNODE *newHEAPNODE(void *v, void (*d)(void *, FILE *), void (*f)(void *)) {
    HEAPNODE *n = malloc(sizeof(HEAPNODE));
    n->key = 0;
    n->value = v;
    n->parent = NULL;
    n->children = newDLL(d, f);
    n->owner = NULL;
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

HEAPNODE *getHEAPNODEparent(HEAPNODE *n) {
    assert(n != 0);
    return n->parent;
}

void setHEAPNODEparent(HEAPNODE *n, HEAPNODE *p) {
    assert(n != 0);
    n->parent = n;
}

DLL *getHEAPNODEchildren(HEAPNODE *n) {
    assert(n != 0);
    return n->children;
}

void setHEAPNODEchildren(HEAPNODE *n, DLL *children) {
    assert(n != 0);
    n->children = children;
}

DLL *getHEAPNODEowner(HEAPNODE *n) {
    assert(n != 0);
    return n->owner;
}

void setHEAPNODEowner(HEAPNODE *n, DLL *owner) {
    assert(n != 0);
    n->owner = owner;
}

void displayHEAPNODE(void *v, FILE *fp) {
}

int isRoot(HEAPNODE *n) {
    assert(n != 0);
    return getHEAPNODEparent(n) == n ? 1 : 0;
}


/* BINOMIAL private method prototypes */
HEAPNODE *combine(BINOMIAL *b, HEAPNODE *x, HEAPNODE *y);


struct BINOMIAL {
    DLL *rootlist;
    HEAPNODE *extreme;
    int size;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*update)(void *, void *);
    void (*free)(void *);
    HEAPNODE *(*combine)(BINOMIAL *, HEAPNODE *, HEAPNODE *);
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

void *insertBINOMIAL(BINOMIAL *b, void *v) {
    assert(b != 0);
    HEAPNODE *n = newHEAPNODE(v, b->display, b->free);
    setHEAPNODEparent(n, n);
    insertDLL(b->rootlist, 0, n);
    b->size++;
    // TODO: Consolidate rootlist
    return n;
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


/* Private Method Definitions */

HEAPNODE *combine(BINOMIAL *b, HEAPNODE *x, HEAPNODE *y) {
    assert(b != 0);
    assert(x != 0);
    assert(y != 0);
    if (b->compare(getHEAPNODEvalue(x), getHEAPNODEvalue(y)) < 0) {
        insertDLL(getHEAPNODEchildren(x), 0, y);
        setHEAPNODEparent(y, x);
        return x;
    }
    else {
        insertDLL(getHEAPNODEchildren(y), 0, x);
        setHEAPNODEparent(x, y);
        return y;
    }
}
