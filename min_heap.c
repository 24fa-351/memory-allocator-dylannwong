#include <stdio.h>
#include <unistd.h>

#include "min_heap.h"

heap global_heap = {NULL, NULL, 0};
void* get_me_blocks( ssize_t how_much ) {

    void* ptr = sbrk(0);
    if (sbrk(how_much) == (void*)-1) {
        perror("sbrk");
        return NULL;
    }
    return ptr;

}

void heap_insert(int size, char* pointer_to_start) {
    chunk_on_heap* new_chunk = (chunk_on_heap*)(get_chunk(sizeof(chunk_on_heap)));
    new_chunk->size = size;
    new_chunk->pointer_to_start = pointer_to_start;
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
        while(current->next != NULL ) {
            if(current->next->size >= size) {
                new_chunk->next = current->next;
                new_chunk->prev = current;
                current->next->prev = new_chunk;
                current->next = new_chunk;
                return;
            }
            current = current->next;
        }
    }
    global_heap.size++;
}

//takes chunk from heap and returns a pointer to it
void* get_chunk(int size) {

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

        void* ptr = get_me_blocks(1024*1024*1024);
        if(ptr == NULL) {
            return NULL;
        }
        heap_insert(1024*1024*1024 - size, (char*)ptr + size);
        
        return ptr;
    

}
void alt_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    // Calculate the size of the chunk to be freed
    // This is a simplified example; in a real allocator, you would need to store the size of each allocated chunk
    int size = 256; // Replace this with the actual size of the chunk

    // Insert the freed chunk back into the heap
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

int main() {
    char* ptr = get_chunk(100);
    if(ptr == NULL) {
        return 1;
    }
    printf("ptr: %p\n", ptr);
    return 0;
}