#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "queue.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(13);
    //simple INTEGER test of BST
    BST *p = newBST(displayINTEGER,compareINTEGER,0,freeINTEGER);
    insertBST(p,newINTEGER(3));
    insertBST(p,newINTEGER(2));
    printf("pre-order: ");
    displayBST(p,stdout);
    printf("\n");
    printf("level: ");
    displayBSTdebug(p,stdout);
    INTEGER *q = newINTEGER(2);
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    INTEGER *r = newINTEGER(3);
    freeBSTNODE(deleteBST(p,r),freeINTEGER);
    printf("pre-order: ");
    displayBST(p,stdout);
    printf("\n");
    printf("level: ");
    displayBSTdebug(p,stdout);
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    setINTEGER(r,2);
    freeBSTNODE(deleteBST(p,r),freeINTEGER);
    printf("pre-order: ");
    displayBST(p,stdout);
    printf("\n");
    printf("level: ");
    displayBSTdebug(p,stdout);
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeBST(p));
    statisticsBST(p,stdout);
    freeINTEGER(q);
    freeINTEGER(r);
    freeBST(p);
    return 0;
    }
