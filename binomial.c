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
#include <math.h>

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
    return n;
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
    n->parent = p;
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

int isRoot(HEAPNODE *n) {
    assert(n != 0);
    return getHEAPNODEparent(n) == n ? 1 : 0;
}

void swap(HEAPNODE *x, HEAPNODE *y) {
    void *tmp = getHEAPNODEvalue(x);
    setHEAPNODEvalue(x, getHEAPNODEvalue(y));
    setHEAPNODEvalue(y, tmp);
}


/* BINOMIAL private method prototypes */
static HEAPNODE *bubbleUp(BINOMIAL *b, HEAPNODE *n);
static HEAPNODE *combine(BINOMIAL *b, HEAPNODE *x, HEAPNODE *y);
static void consolidate(BINOMIAL *b);
static void updateConsolidationArray(BINOMIAL *, HEAPNODE *[], HEAPNODE *);


struct BINOMIAL {
    DLL *rootlist;
    HEAPNODE *extreme;
    int size;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*update)(void *, void *);
    void (*free)(void *);
    HEAPNODE *(*bubbleUp)(BINOMIAL *, HEAPNODE *);
    HEAPNODE *(*combine)(BINOMIAL *, HEAPNODE *, HEAPNODE *);
    void (*consolidate)(BINOMIAL *);
    void (*updateConsolidationArray)(BINOMIAL *, HEAPNODE *[], HEAPNODE *);
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
    rv->bubbleUp = bubbleUp;
    rv->combine = combine;
    rv->consolidate = consolidate;
    rv->updateConsolidationArray = updateConsolidationArray;
    return rv;
}

void *insertBINOMIAL(BINOMIAL *b, void *v) {
    assert(b != 0);
    HEAPNODE *n = newHEAPNODE(v, b->display, b->free);
    setHEAPNODEparent(n, n);
    insertDLL(b->rootlist, 0, n);
    b->size++;
    b->consolidate(b);
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
    recipient->consolidate(recipient);
}

void deleteBINOMIAL(BINOMIAL *b, void *node) {
    assert(b != 0);
    decreaseKeyBINOMIAL(b, node, NULL);
    void *rv = extractBINOMIAL(b);
    (void)rv;
}

void decreaseKeyBINOMIAL(BINOMIAL *b, void *node, void *value) {
    // TODO: Am I correct?
    assert(b != 0);
    assert(node != 0);
    setHEAPNODEvalue(node, value);
    HEAPNODE *rv = b->bubbleUp(b, node);
    if (b->compare(getHEAPNODEvalue(rv), b->extreme) < 0) {
        b->extreme = rv;
    }
}

void *peekBINOMIAL(BINOMIAL *b) {
    assert(b != 0);
    return getHEAPNODEvalue(b->extreme);
}

void *extractBINOMIAL(BINOMIAL *b) {
    // TODO: Am I correct?
    assert(b != 0);
    HEAPNODE *y = b->extreme;
    y = removeDLLnode(b->rootlist, y);
    DLL *yChildren = getHEAPNODEchildren(y);
    firstDLL(yChildren);
    while (currentDLL(yChildren) != NULL) {
        setHEAPNODEparent(currentDLL(yChildren), currentDLL(yChildren));
        nextDLL(yChildren);
    }
    unionDLL(b->rootlist, yChildren);
    b->consolidate(b);
    b->size--;
    void *rv = getHEAPNODEvalue(y);
    freeDLL(yChildren);
    free((HEAPNODE *)y);
    return rv;
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


/******************** Private Method Definitions ********************/

HEAPNODE *bubbleUp(BINOMIAL *b, HEAPNODE *n) {
    assert(b != 0);
    assert(n != 0);
    // TODO: Am I correct?
    // TODO: Refactor?
    HEAPNODE *p = getHEAPNODEparent(n);
    if (n == p) return n;
    if (b->compare(getHEAPNODEvalue(n), getHEAPNODEvalue(getHEAPNODEparent(n))) >= 0) {
        return n;
    }
    if (b->update) b->update(getHEAPNODEvalue(n), p);
    if (b->update) b->update(getHEAPNODEvalue(p), n);
    void *tmp = getHEAPNODEvalue(n);
    setHEAPNODEvalue(n, getHEAPNODEvalue(p));
    setHEAPNODEvalue(p, tmp);
    return bubbleUp(b, p);
}

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

void consolidate(BINOMIAL *b) {
    // TODO: Am I correct?
    assert(b != 0);
    int size = (log10(b->size) / log10(2)) + 1;
    HEAPNODE *D[size];
    for (int i = 0; i < size; ++i) {
        D[i] = NULL;
    }
    while (sizeDLL(b->rootlist) > 0) {
        HEAPNODE *spot = getDLL(b->rootlist, 0);
        void *trash = removeDLLnode(b->rootlist, spot);
        (void)trash; // Shut the compiler up
        b->updateConsolidationArray(b, D, spot);
    }
    b->extreme = NULL;
    for (int i = 0; i < size; ++i) {
        if (D[i] != NULL) {
            insertDLL(b->rootlist, 0, D[i]);
            if (b->extreme == NULL || b->compare(D[i], b->extreme) < 0) {
                b->extreme = D[i];
            }
        }
    }
}

void updateConsolidationArray(BINOMIAL *b, HEAPNODE *D[], HEAPNODE *spot) {
    // TODO: Refactor?
    assert(b != 0);
    assert(spot != 0);
    int degree = sizeDLL(getHEAPNODEchildren(spot));
    while (D[degree] != NULL) {
        b->combine(b, spot, D[degree]);
        D[degree] = NULL;
        degree++;
    }
    D[degree] = spot;
}
