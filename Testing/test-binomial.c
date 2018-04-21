#include <stdio.h>
#include "../integer.h"
#include "../dll.h"
#include "../binomial.h"

int main(void) {
    BINOMIAL *b = newBINOMIAL(displayINTEGER, compareINTEGER, 0, freeINTEGER);

    for (int i = 0; i <= 12; i++) {
        insertBINOMIAL(b, newINTEGER(i));
    }
    displayBINOMIALdebug(b, stdout);
    for (int j = 0; j <= 13; j++) {
        printf("REMOVING: ");
        displayINTEGER(extractBINOMIAL(b), stdout);
        printf("\n");
        displayBINOMIALdebug(b, stdout);
    }

    freeBINOMIAL(b);
    return 0;
}
