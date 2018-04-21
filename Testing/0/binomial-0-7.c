#include <stdio.h>
#include <stdlib.h>
#include "../../integer.h"
#include "../../real.h"
#include "../../string.h"
#include "../../binomial.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(8);
    //INTEGER test of BINOMIAL
    int i;
    BINOMIAL *p = newBINOMIAL(displayINTEGER,rcompareINTEGER,0,freeINTEGER);
    printf("MAX HEAP\n");
    for (i = 0; i < 1096; ++i)
        {
        int j = random();
        INTEGER *a = newINTEGER(j);
        insertBINOMIAL(p,a);
        }
    if (sizeBINOMIAL(p) < 200)
        {
        displayBINOMIAL(p,stdout);
        printf("debug:\n");
        displayBINOMIALdebug(p,stdout);
        }
    printf("size is %d\n",sizeBINOMIAL(p));
    freeBINOMIAL(p);
    return 0;
    }
