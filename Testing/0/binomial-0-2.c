#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "binomial.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(3);
    //simple STRING test of BINOMIAL
    BINOMIAL *p = newBINOMIAL(displaySTRING,compareSTRING,0,freeSTRING);
    printf("MIN HEAP\n");
    insertBINOMIAL(p,newSTRING("a3"));
    insertBINOMIAL(p,newSTRING("a0"));
    insertBINOMIAL(p,newSTRING("a2"));
    displayBINOMIAL(p,stdout);
    printf("debug:\n");
    displayBINOMIALdebug(p,stdout);
    printf("min should be %s\n",getSTRING(peekBINOMIAL(p)));
    STRING *r = extractBINOMIAL(p);
    printf("min is %s\n",getSTRING(r));
    freeSTRING(r);
    displayBINOMIAL(p,stdout);
    printf("debug:\n");
    displayBINOMIALdebug(p,stdout);
    printf("min should be %s\n",getSTRING(peekBINOMIAL(p)));
    r = extractBINOMIAL(p);
    printf("min is %s\n",getSTRING(r));
    freeSTRING(r);
    displayBINOMIAL(p,stdout);
    printf("debug:\n");
    displayBINOMIALdebug(p,stdout);
    printf("peek: %s\n",getSTRING(peekBINOMIAL(p)));
    printf("size: %d\n",sizeBINOMIAL(p));
    freeBINOMIAL(p);
    return 0;
    }
