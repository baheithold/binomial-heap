#include <stdio.h>
#include "integer.h"
#include "dll.h"

int main(void) {
    DLL *list = newDLL(displayINTEGER, freeINTEGER);

    void *v = insertDLL(list, 0, newINTEGER(7));
    v = insertDLL(list, 0, newINTEGER(7));

    freeDLL(list);
    return 0;
}
