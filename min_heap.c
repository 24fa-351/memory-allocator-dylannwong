#include <stdio.h>
#include <unistd.h>

#include "min_heap.h"

#define SBRK (1024*1024)

heap global_heap = {NULL, NULL, 0};

void* get_me_blocks( ssize_t how_much ) {

    void* ptr = sbrk(0);
    sbrk(how_much);
    return ptr;

}
//prints out the heap
void print_heap() {
    printf("Heap state:\n");
    chunk_on_heap* current = global_heap.head;
    int counter = 0;

    while (current != NULL) {
        fprintf(stderr, "Chunk: size=%d, pointer_to_start=%p, next=%p\n",
                current->size, current->pointer_to_start, current->next);

        counter++;
        if (counter > global_heap.size) {
            fprintf(stderr, "Error: Detected a loop in the heap!\n");
            break;
        }

        current = current->next;
    }
    fprintf(stderr, "Heap size: %d\n", global_heap.size);
}
//inserts a chunk into correct position in the heap
void heap_insert(int size, char* pointer_to_start) {

    printf("Inserting chunk of size %d at %p\n", size, pointer_to_start);

    chunk_on_heap* new_chunk = (chunk_on_heap*)pointer_to_start;

    new_chunk->size = size;
    new_chunk->pointer_to_start = (char*)pointer_to_start + sizeof(chunk_on_heap);
    new_chunk->next = NULL;
    new_chunk->prev = NULL;

    if (global_heap.head == NULL) {
        global_heap.head = new_chunk;
        global_heap.tail = new_chunk;
    } else {
        chunk_on_heap* current = global_heap.head;
        chunk_on_heap* previous = NULL;

        while (current != NULL && current->size < size) {
            previous = current;
            current = current->next;
        }
        if (previous == NULL) {
            new_chunk->next = global_heap.head;
            global_heap.head->prev = new_chunk;
            global_heap.head = new_chunk;
        } else if (current == NULL) {
            previous->next = new_chunk;
            new_chunk->prev = previous;
            global_heap.tail = new_chunk;
        } else {
            new_chunk->next = current;
            new_chunk->prev = previous;
            previous->next = new_chunk;
            current->prev = new_chunk;
        }
    }

    global_heap.size++;
}
//gets the smallest avaliable chunk of memory if not sbrks a large one
void* get_chunk(int size) {
    printf("Requesting chunk of size %d\n", size);

    chunk_on_heap* current = global_heap.head;

    while (current != NULL) {
        if (current->size >= size + sizeof(chunk_on_heap)) {
            void* chunk = current->pointer_to_start;

            if (current->size == size) {
                heap_remove(current);
            } else {
                current->pointer_to_start += (size + sizeof(chunk_on_heap));
                current->size -= size + sizeof(chunk_on_heap);
            }

            printf("Returning chunk at %p\n", chunk);
            return chunk;
        }
        current = current->next;
    }

    fprintf(stderr, "No chunk of size %d available\n", size);

    void* ptr = get_me_blocks(SBRK);
    if (ptr == NULL) {
        return NULL;
    }

    chunk_on_heap* new_chunk = (chunk_on_heap*)ptr;
    new_chunk->size = SBRK - sizeof(chunk_on_heap);
    new_chunk->pointer_to_start = (char*)ptr + sizeof(chunk_on_heap);
    new_chunk->next = NULL;
    new_chunk->prev = NULL;

    heap_insert(new_chunk->size, (char*)new_chunk);

    return get_chunk(size );
}

//inserts free chunk back into heap 
void free_chunk(void* ptr, int size) {
    printf("Freeing chunk at %p of size %d\n", ptr, size);

    heap_insert(size, (char*)ptr);
}
//removes a chunk that is perfectly utilized
void heap_remove(chunk_on_heap* chunk) {
    if (chunk == NULL) return;

    if (chunk->prev == NULL) {
        global_heap.head = chunk->next;
    } else {
        chunk->prev->next = chunk->next;
    }

    if (chunk->next == NULL) {
        global_heap.tail = chunk->prev;
    } else {
        chunk->next->prev = chunk->prev;
    }

    chunk->next = NULL;
    chunk->prev = NULL;

    global_heap.size--;
}