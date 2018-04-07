#include <stdio.h>
#include "../integer.h"
#include "../dll.h"
#include "../binomial.h"

int main(void) {
    BINOMIAL *b = newBINOMIAL(displayINTEGER, compareINTEGER, 0, freeINTEGER);

    for (int i = 0; i <= 12; i++) {
        printf("INSERTING: %d\n", i);
        insertBINOMIAL(b, newINTEGER(i));
    }

    return 0;
}
