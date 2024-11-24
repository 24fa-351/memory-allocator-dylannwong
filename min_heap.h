#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stddef.h>

typedef struct chunk_on_heap {
  int size;
  char *pointer_to_start;
  struct chunk_on_heap *next;
  struct chunk_on_heap *prev;
} chunk_on_heap;

typedef struct{
  chunk_on_heap *head;
  chunk_on_heap *tail;
  chunk_on_heap *data;
  int size;
} heap;
void free_chunk(void* ptr, int size);

void heap_remove(chunk_on_heap *chunk);

void *get_me_blocks(ssize_t how_much);

void heap_insert(int size, char *pointer_to_start);

void *get_chunk(int size);

#endif