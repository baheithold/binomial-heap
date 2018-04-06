#include <stdio.h>
#include "integer.h"
#include "dll.h"

int main(void) {
    DLL *list = newDLL(displayINTEGER, freeINTEGER);

    freeDLL(list);
    return 0;
}
