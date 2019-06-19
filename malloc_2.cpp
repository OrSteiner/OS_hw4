//
// Created by orsht on 6/16/2019.
//

#include <unistd.h>
#include <stdlib.h>
#include <cstring>




struct metadata{
    metadata* next;
    metadata* prev;
    size_t size;
    bool is_free;
    void* address;
};

metadata* head = NULL;
metadata* tail = NULL;
size_t num_free_blocks = 0;
size_t num_allocated_blocks = 0;
size_t num_free_bytes = 0;
size_t num_allocated_bytes = 0;


void* malloc(size_t size){
    if(size == 0 || size > 100000000){
        return NULL;
    }
    metadata* iterator = head;
    while(iterator){
        if(iterator->is_free && iterator->size > size){
            iterator->is_free = false;
            --num_free_blocks;
            num_free_bytes -= iterator->size;
            return iterator->address;
        }
        iterator = iterator->next;
    }
    metadata* metadata_pointer = (metadata*)sbrk(sizeof(metadata));
     
    if(*((int*)metadata_pointer) == -1){
        return NULL;
    }
    metadata_pointer->next = NULL;
    metadata_pointer->prev = tail;
    metadata_pointer->size = size;
    metadata_pointer->is_free = false;
    metadata_pointer->address = NULL;
    if(!head){
        head = metadata_pointer;
        tail = metadata_pointer;
    }
    else {
        tail->next = metadata_pointer;
        tail = tail->next;
    }
    void* address = sbrk(size);
    if(*((int*)address) == -1){
        return NULL;
    }
    metadata_pointer->address = address;
    ++num_allocated_blocks;
    num_allocated_bytes += size;
    return address;
}

void* calloc(size_t num, size_t size){
    void* address = malloc(num*size);
    if(!address){
        return NULL;
    }
    std::memset(address, 0, num*size);
    return address;
}

void free(void* p){
    if(!p)
        return;
    metadata* iterator = head;
    while(iterator){
        if(iterator->address == p){
            if(iterator->is_free)
                return;
            iterator->is_free = true;
            ++num_free_blocks;
            num_free_bytes += iterator->size;
        }
        iterator = iterator->next;
    }
}

void* realloc(void* oldp, size_t size){
    metadata* iterator = head;
    while(iterator){
        if(iterator->address == oldp){
            if(size <= iterator->size){
                return iterator->address;
            }
            else
                break;
        }
        iterator = iterator->next;
    }
    void* pointer = malloc(size);
    if(!pointer)
        return NULL;
    if(!oldp)
        return pointer;
    memcpy(pointer, oldp, size);
    free(oldp);
    return pointer;
}

size_t _num_free_blocks(){
    return num_free_blocks;
}

size_t _num_free_bytes(){
    return num_free_bytes;
}

size_t _num_allocated_blocks(){
    return num_allocated_blocks;
}

size_t _num_allocated_bytes(){
    return num_allocated_bytes;
}

size_t _num_meta_data_bytes(){
    return sizeof(metadata)*num_allocated_blocks;
}

size_t _size_meta_data(){
    return sizeof(metadata);
}



