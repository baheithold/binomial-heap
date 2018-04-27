#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "integer.h"

struct INTEGER {
    int value;
    void *owner;
};

INTEGER *newINTEGER(int x) {
    INTEGER *p = malloc(sizeof(INTEGER));
    assert(p != 0);
    p->value = x;
    p->owner = NULL;
    return p;
}

int getINTEGER(INTEGER *v) {
    return v->value;
}

int setINTEGER(INTEGER *v,int x) {
    int old = v->value;
    v->value = x;
    return old;
}

void *getINTEGERowner(INTEGER *v) {
    assert(v != 0);
    return v->owner;
}

void setINTEGERowner(INTEGER *v, void *o) {
    assert(v != 0);
    v->owner = o;
}

void updateINTEGER(void *v, void *o) {
    assert(v != 0);
    INTEGER *p = v;
    p->owner = o;
}

void displayINTEGER(void *v,FILE *fp) {
    fprintf(fp,"%d",getINTEGER((INTEGER *) v));
}

int compareINTEGER(void *v,void *w) {
    return getINTEGER(v) - getINTEGER(w);
}

int rcompareINTEGER(void *v,void *w) {
    return getINTEGER(w) - getINTEGER(v);
}

void freeINTEGER(void *v) {
    free((INTEGER *) v);
}
