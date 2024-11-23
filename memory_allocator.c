#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "memory_allocator.h"
#include "min_heap.h"

//allocate memory


char* alt_malloc(size_t size) {
    //void* ptr = get_me_blocks(sbrk_size);

    char* ptr = get_chunk(size);

    if(ptr == NULL) {
        fprintf(stderr, __FILE__ ":%d malloc failed\n", __LINE__);
        exit(1);
    }
 
    fprintf(stderr, __FILE__ ":%d malloc(%lu) = %p\n", __LINE__, size, ptr);
    return ptr;
}

void alt_free(void* ptr) {
    fprintf(stderr, __FILE__ ": free(%p)\n", ptr);
    free(ptr);
}

//malloc but multiply the size and num
char *alt_calloc(size_t num, size_t size) {
    return malloc(num * size);
}
//take allocated ptr and allocate to new size
void* alt_realloc(void* ptr, size_t size) {
    //one line
    NULL;
}