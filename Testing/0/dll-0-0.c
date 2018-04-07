#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "integer.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(9383);
    DLL *p = newDLL(displayINTEGER,freeINTEGER);
    insertDLL(p,0,newINTEGER(3));
    insertDLL(p,sizeDLL(p),newINTEGER(2));
    displayDLL(p,stdout);
    printf("\n");
    freeINTEGER(removeDLL(p,1));
    displayDLL(p,stdout);
    printf("\n");
    freeINTEGER(removeDLL(p,0));
    displayDLL(p,stdout);
    printf("\n");
    DLL *q = newDLL(displayINTEGER,freeINTEGER);
    insertDLL(p,0,newINTEGER(6));
    insertDLL(p,0,newINTEGER(7));
    insertDLL(q,0,newINTEGER(4));
    insertDLL(q,0,newINTEGER(5));
    displayDLL(p,stdout);
    printf("\n");
    displayDLL(q,stdout);
    printf("\n");
    unionDLL(p,q);
    displayDLL(p,stdout);
    displayDLL(q,stdout);
    printf("\n");
    displayDLLdebug(p,stdout);
    printf("\n");
    displayDLLdebug(q,stdout);
    printf("\n");
    freeDLL(p);
    freeDLL(q);
    return 0;
    }
