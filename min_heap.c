#include <stdio.h>
#include <unistd.h>

#include "min_heap.h"

#define SBRK 1024*1024

heap global_heap = {NULL, NULL, 0};
void* get_me_blocks( ssize_t how_much ) {

    void* ptr = sbrk(0);
    if (sbrk(how_much) == (void*)-1) {
        perror("sbrk");
        return NULL;
    }
    return ptr;

}

void print_heap() {
    printf("Heap state:\n");
    chunk_on_heap* current = global_heap.head;
    while (current != NULL) {
        printf("Chunk: size=%d, pointer_to_start=%p\n", current->size, current->pointer_to_start);
        current = current->next;
    }
    printf("heap size: %d\n", global_heap.size);
}

void heap_insert(int size, char* pointer_to_start) {
    printf("Inserting chunk of size %d at %p\n", size, pointer_to_start);

    chunk_on_heap* new_chunk = (chunk_on_heap*)(pointer_to_start);
    fprintf(stderr, "new_chunk: %p\n", new_chunk);
    new_chunk->size = size;
    new_chunk->pointer_to_start = pointer_to_start + sizeof(chunk_on_heap);
    new_chunk->next = NULL;
    new_chunk->prev = NULL;
    
    if(global_heap.head == NULL) {
        global_heap.head = new_chunk;
        global_heap.tail = new_chunk;
    } else if(global_heap.head->size >= size) {
        new_chunk->next = global_heap.head;
        global_heap.head->prev = new_chunk;
        global_heap.head = new_chunk;
    } else if(global_heap.tail->size <= size) {
        new_chunk->prev = global_heap.tail;
        global_heap.tail->next = new_chunk;
        global_heap.tail = new_chunk; 
    } else {
        chunk_on_heap* current = global_heap.head;
        while (current->next != NULL && current->next->size < size) {
            current = current->next;
        }
        new_chunk->next = current->next;
        new_chunk->prev = current;
        if (current->next != NULL) {
            current->next->prev = new_chunk;
        }
        current->next = new_chunk;
    }
    global_heap.size++;
}

//takes chunk from heap and returns a pointer to it
void* get_chunk(int size) {
    printf("Requesting chunk of size %d\n", size);


    chunk_on_heap* current = global_heap.head;

    while(current != NULL) {
        if(current->size >= size) {
            void* chunk = current->pointer_to_start;
            current->pointer_to_start += size;
            current->size -= size;

            if(current->size == 0) {
                heap_remove(current);
            }
            return chunk;
        }
        current = current->next;
    }
        fprintf(stderr, "No chunk of size %d available\n", size);
        void* ptr = get_me_blocks(SBRK);
        if(ptr == NULL) {
            return NULL;
        }
        heap_insert(SBRK - size, (char*)ptr + size);
        printf("Returning new chunk at %p\n", ptr);

        return ptr;
    

}

void free_chunk(void* ptr, int size) {
    printf("Freeing chunk at %p of size %d\n", ptr, size);
    heap_insert(size, (char*)ptr);
}

void heap_remove(chunk_on_heap *chunk) {
    if(chunk->prev == NULL) {
        global_heap.head = chunk->next;
    } else {
        chunk->prev->next = chunk->next;
    }

    if(chunk->next == NULL) {
        global_heap.tail = chunk->prev;
    } else {
        chunk->next->prev = chunk->prev;
    }
    global_heap.size--;
}
/*
int main() {
    char* ptr1 = get_chunk(100);
    char* ptr2 = get_chunk(200);
    print_heap();
    free_chunk(ptr1, 100);
    free_chunk(ptr2, 200);
    print_heap();
    return 0;
}*/