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
    srandom(2);
    //simple REAL test of BINOMIAL
    BINOMIAL *p = newBINOMIAL(displayREAL,compareREAL,0,freeREAL);
    printf("MIN HEAP\n");
    insertBINOMIAL(p,newREAL(3));
    insertBINOMIAL(p,newREAL(2));
    insertBINOMIAL(p,newREAL(0));
    displayBINOMIAL(p,stdout);
    printf("debug:\n");
    displayBINOMIALdebug(p,stdout);
    printf("min should be %f\n",getREAL(peekBINOMIAL(p)));
    REAL *r = extractBINOMIAL(p);
    printf("min is %f\n",getREAL(r));
    freeREAL(r);
    displayBINOMIAL(p,stdout);
    printf("debug:\n");
    displayBINOMIALdebug(p,stdout);
    printf("min should be %f\n",getREAL(peekBINOMIAL(p)));
    r = extractBINOMIAL(p);
    printf("min is %f\n",getREAL(r));
    freeREAL(r);
    displayBINOMIAL(p,stdout);
    printf("debug:\n");
    displayBINOMIALdebug(p,stdout);
    printf("peek: %f\n",getREAL(peekBINOMIAL(p)));
    printf("size: %d\n",sizeBINOMIAL(p));
    freeBINOMIAL(p);
    return 0;
    }
