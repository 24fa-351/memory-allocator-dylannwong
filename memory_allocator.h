#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

char* alt_malloc(size_t size);
void alt_free(void* ptr);
char* alt_calloc(size_t num, size_t size);
void* alt_realloc(void* ptr, size_t size);

#endif