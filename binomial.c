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

typedef struct BHNODE BHNODE;

struct BHNODE {
    int key;
    void *value;
    BHNODE *parent;
    DLL *children;
    DLL *owner;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*free)(void *);
};

BHNODE *newBHNODE(
        void *v,
        void (*d)(void *, FILE *),
        int (*c)(void *, void *),
        void (*f)(void *)) {
    BHNODE *n = malloc(sizeof(BHNODE));
    assert(n != 0);
    n->key = 0;
    n->value = v;
    n->parent = NULL;
    n->children = newDLL(d, f);
    n->owner = NULL;
    n->display = d;
    n->compare = c;
    n->free = f;
    return n;
}

int getBHNODEkey(BHNODE *n) {
    assert(n != 0);
    return n->key;
}

void setBHNODEkey(BHNODE *n, int key) {
    assert(n != 0);
    n->key = key;
}

void *getBHNODEvalue(BHNODE *n) {
    assert(n != 0);
    return n->value;
}

void setBHNODEvalue(BHNODE *n, void *v) {
    assert(n != 0);
    n->value = v;
}

BHNODE *getBHNODEparent(BHNODE *n) {
    assert(n != 0);
    return n->parent;
}

void setBHNODEparent(BHNODE *n, BHNODE *p) {
    assert(n != 0);
    n->parent = p;
}

DLL *getBHNODEchildren(BHNODE *n) {
    assert(n != 0);
    return n->children;
}

void setBHNODEchildren(BHNODE *n, DLL *children) {
    assert(n != 0);
    n->children = children;
}

DLL *getBHNODEowner(BHNODE *n) {
    assert(n != 0);
    return n->owner;
}

void setBHNODEowner(BHNODE *n, DLL *owner) {
    assert(n != 0);
    n->owner = owner;
}

void displayBHNODE(void *n, FILE *fp) {
    BHNODE *x = n;
    x->display(x->value, fp);
}

int compareBHNODE(void *a, void *b) {
    assert(a != 0);
    assert(b != 0);
    void *av = getBHNODEvalue(a);
    void *bv = getBHNODEvalue(b);
    return ((BHNODE *) a)->compare(av, bv);
}

void freeBHNODE(void *n) {
    assert(n != 0);
    BHNODE *x = n;
    freeDLL(getBHNODEchildren(x));
    if (x->free) {
        x->free(getBHNODEvalue(x));
    }
    free(x);
}

int isRoot(BHNODE *n) {
    assert(n != 0);
    return getBHNODEparent(n) == n ? 1 : 0;
}

void swap(BHNODE *x, BHNODE *y) {
    void *tmp = getBHNODEvalue(x);
    setBHNODEvalue(x, getBHNODEvalue(y));
    setBHNODEvalue(y, tmp);
}


/* BINOMIAL private method prototypes */
static BHNODE *bubbleUp(BINOMIAL *b, BHNODE *n);
static BHNODE *combine(BINOMIAL *b, BHNODE *x, BHNODE *y);
static void consolidate(BINOMIAL *b);
static void updateConsolidationArray(BINOMIAL *, BHNODE *[], BHNODE *);


struct BINOMIAL {
    DLL *rootlist;
    BHNODE *extreme;
    int size;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*update)(void *, void *);
    void (*free)(void *);
    BHNODE *(*bubbleUp)(BINOMIAL *, BHNODE *);
    BHNODE *(*combine)(BINOMIAL *, BHNODE *, BHNODE *);
    void (*consolidate)(BINOMIAL *);
    void (*updateConsolidationArray)(BINOMIAL *, BHNODE *[], BHNODE *);
};

BINOMIAL *newBINOMIAL(
        void (*display)(void *, FILE *),
        int (*compare)(void *, void *),
        void (*update)(void *, void *),
        void (*free)(void *)) {
    BINOMIAL *rv = malloc(sizeof(BINOMIAL));
    assert(rv != 0);
    rv->rootlist = newDLL(displayBHNODE, freeBHNODE);
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
    BHNODE *n = newBHNODE(v, b->display, b->compare, b->free);
    setBHNODEparent(n, n);
    insertDLL(b->rootlist, sizeDLL(b->rootlist), n);
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
    setBHNODEvalue(node, value);
    BHNODE *rv = b->bubbleUp(b, node);
    if (b->compare(getBHNODEvalue(rv), b->extreme) > 0) {
        b->extreme = rv;
    }
}

void *peekBINOMIAL(BINOMIAL *b) {
    assert(b != 0);
    return getBHNODEvalue(b->extreme);
}

void *extractBINOMIAL(BINOMIAL *b) {
    // TODO: Am I correct?
    assert(b != 0);
    BHNODE *y = b->extreme;
    y = removeDLLnode(b->rootlist, y);
    DLL *yChildren = getBHNODEchildren(y);
    firstDLL(yChildren);
    while (currentDLL(yChildren) != NULL) {
        setBHNODEparent(currentDLL(yChildren), currentDLL(yChildren));
        nextDLL(yChildren);
    }
    unionDLL(b->rootlist, yChildren);
    b->consolidate(b);
    b->size--;
    void *rv = getBHNODEvalue(y);
    freeDLL(yChildren);
    free((BHNODE *)y);
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

void displayBINOMIAL(BINOMIAL *b, FILE *fp) {
    assert(b != 0);
    fprintf(fp, "rootlist: ");
    int degree = 0;
    int spotDegree;
    firstDLL(b->rootlist);
    while (moreDLL(b->rootlist)) {
        DLL *spotChildren = getBHNODEchildren(currentDLL(b->rootlist));
        spotDegree = sizeDLL(spotChildren);
        if (degree < spotDegree) {
            fprintf(fp, "NULL");
        }
        else {
            displayBHNODE(currentDLL(b->rootlist), fp);
            if (currentDLL(b->rootlist) == b->extreme) fprintf(fp, "*");
            nextDLL(b->rootlist);
        }
        if (moreDLL(b->rootlist)) fprintf(fp, " ");
        degree++;
    }
}

void displayBINOMIALdebug(BINOMIAL *b, FILE *fp) {
    assert(b != 0);
    displayDLL(b->rootlist, fp);
    firstDLL(b->rootlist);
    while (moreDLL(b->rootlist)) {
        fprintf(fp, "\n");
        displayDLL(getBHNODEchildren(currentDLL(b->rootlist)), fp);
        nextDLL(b->rootlist);
    }
}

void freeBINOMIAL(BINOMIAL *b) {
    assert(b != 0);
    firstDLL(b->rootlist);
    freeDLL(b->rootlist);
    free(b);
}


/******************** Private Method Definitions ********************/

BHNODE *bubbleUp(BINOMIAL *b, BHNODE *n) {
    assert(b != 0);
    assert(n != 0);
    // TODO: Am I correct?
    // TODO: Refactor?
    BHNODE *p = getBHNODEparent(n);
    if (n == p) return n;
    if (compareBHNODE(n, getBHNODEparent(n)) >= 0) {
        return n;
    }
    if (b->update) b->update(getBHNODEvalue(n), p);
    if (b->update) b->update(getBHNODEvalue(p), n);
    void *tmp = getBHNODEvalue(n);
    setBHNODEvalue(n, getBHNODEvalue(p));
    setBHNODEvalue(p, tmp);
    return bubbleUp(b, p);
}

BHNODE *combine(BINOMIAL *b, BHNODE *x, BHNODE *y) {
    assert(b != 0);
    assert(x != 0);
    assert(y != 0);
    if (compareBHNODE(x, y) < 0) {
        DLL *xChildren = getBHNODEchildren(x);
        insertDLL(xChildren, sizeDLL(xChildren), y);
        setBHNODEparent(y, x);
        return x;
    }
    else {
        DLL *yChildren = getBHNODEchildren(y);
        insertDLL(yChildren, sizeDLL(yChildren), x);
        setBHNODEparent(x, y);
        return y;
    }
}

void consolidate(BINOMIAL *b) {
    // TODO: Am I correct?
    assert(b != 0);
    int size = (log(b->size) / log(2)) + 1;
    BHNODE *D[size];
    for (int i = 0; i < size; ++i) {
        D[i] = NULL;
    }
    while (sizeDLL(b->rootlist) > 0) {
        BHNODE *spot = removeDLL(b->rootlist, 0);
        b->updateConsolidationArray(b, D, spot);
    }
    b->extreme = NULL;
    for (int i = 0; i < size; ++i) {
        if (D[i] != NULL) {
            insertDLL(b->rootlist, sizeDLL(b->rootlist), D[i]);
            if (b->extreme == NULL || compareBHNODE(D[i], b->extreme) < 0) {
                b->extreme = D[i];
            }
        }
    }
}

void updateConsolidationArray(BINOMIAL *b, BHNODE *D[], BHNODE *spot) {
    // TODO: Am I correct?
    assert(b != 0);
    assert(D != 0);
    assert(spot != 0);
    int degree = sizeDLL(getBHNODEchildren(spot));
    while (D[degree] != NULL) {
        spot = b->combine(b, spot, D[degree]);
        D[degree] = NULL;
        degree++;
    }
    D[degree] = spot;
}
