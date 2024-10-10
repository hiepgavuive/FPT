#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************************
* Prototypes
******************************************************************************/
bool allocate10Bytes(uint8_t *outPtr) {
    if (outPtr == NULL) {
        return false;
    }

    uint8_t* tempPtr = (uint8_t*)malloc(10 * sizeof(uint8_t));
    if (tempPtr == NULL) {
        return false;
    }

    outPtr = tempPtr;
    return true;
}

/*******************************************************************************
* Code
******************************************************************************/
int main(int argc, char* argv[]) {
    uint8_t* pointer = NULL;

    if (allocate10Bytes(pointer)) {
        printf("Memory allocation successful.\n");
    }
    else {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* kiem tra xem da cap phat chua */ 
    if (pointer != NULL) {
        for (int i = 0; i < 10; i++) {
            pointer[i] = 0;
        }

        for (int i = 0; i < 10; i++) {
            printf("pointer[%d] = %d\n", i, pointer[i]);
        }
    }

    free(pointer);

    return 0;
}
