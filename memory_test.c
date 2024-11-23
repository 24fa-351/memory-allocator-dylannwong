#include <stdio.h>
#include <stdlib.h>
#include "memory_allocator.h"


int main() {

int* arr = (int*)malloc(10 * sizeof(int));


if (arr != NULL) {
  printf("Memory allocated\n");
} else {
    printf("Memory not allocated\n");
    return 1;
}

free(arr);

return 0;

}