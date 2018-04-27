#include <stdio.h>
#include "../integer.h"
#include "../dll.h"
#include "../binomial.h"

int main(void) {
    BINOMIAL *b = newBINOMIAL(displayINTEGER, compareINTEGER, updateINTEGER, freeINTEGER);

    for (int i = 0; i <= 12; i++) {
        insertBINOMIAL(b, newINTEGER(i));
    }
    displayBINOMIALdebug(b, stdout);
    for (int j = 0; j <= 12; j++) {
        freeINTEGER(extractBINOMIAL(b));
    }

    freeBINOMIAL(b);
    return 0;
}
