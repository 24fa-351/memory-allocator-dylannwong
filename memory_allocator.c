#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "memory_allocator.h"
#include "min_heap.h"

typedef struct {
    void* ptr;
    size_t size;
} allocation;

allocation allocations[1000];
size_t num_ptrs = 0;

char* alt_malloc(size_t size) {

    char* ptr = get_chunk(size);

    if(ptr == NULL) {
        fprintf(stderr, __FILE__ ":%d malloc failed\n", __LINE__);
        exit(1);
    }
 
    fprintf(stderr, __FILE__ ":%d malloc(%lu) = %p\n", __LINE__, size, ptr);
    
    allocations[num_ptrs].ptr = ptr;
    allocations[num_ptrs].size = size;
    num_ptrs++;

    return ptr;
}

void alt_free(void* ptr) {
    fprintf(stderr, __FILE__ ": free(%p)\n", ptr);
    for(size_t i = 0; i < num_ptrs; i++) {
        if(allocations[i].ptr == ptr) {
            free_chunk(ptr, allocations[i].size);
            allocations[i] = allocations[num_ptrs - 1];
            num_ptrs--;
            return;
        }
    }
    fprintf(stderr, "Pointer not found\n");

}

//malloc but multiply the size and num
char* alt_calloc(size_t num, size_t size) {
    return malloc(num * size);
}
//take allocated ptr and allocate to new size
void* alt_realloc(void* ptr, size_t size) {

    for (int i = 0; i < num_ptrs; i++) {
        if (allocations[i].ptr == ptr) {
            void* new_ptr = alt_malloc(size);
            if (new_ptr == NULL) {
                return NULL;
            }
            memcpy(new_ptr, ptr, allocations[i].size < size ? allocations[i].size : size);
            alt_free(ptr);
            return new_ptr;
        }
    }
    fprintf(stderr, "Pointer not found\n");
    return NULL;
}