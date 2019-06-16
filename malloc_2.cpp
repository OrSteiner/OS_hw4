//
// Created by orsht on 6/16/2019.
//

#include <unistd.h>
#include <Map_List.h>
#include <list_node.h>
#include <stdlib.h>
#include <cstring>

struct metadata{
public:
    metadata* next;
    metadata* prev;
    size_t size;
    bool is_free;
    metadata* address;
};

metadata* head = nullptr;
metadata* tail = nullptr;
size_t num_free_blocks = 0;
size_t num_allocated_bolcks = 0;
size_t num_free_bytes = 0;
size_t num_allocated_bytes = 0;

void* malloc(size_t size){
    if(size == 0 || size > 100000000){
        return nullptr;
    }
    metadata* iterator = head;
    while(!iterator){
        if(iterator->is_free && iterator->size > size){
            iterator->is_free = false;
            --num_free_blocks;
            num_free_bytes -= size;
            return iterator->address;
        }
        iterator = iterator->next;
    }
    auto metadata_pointer = sbrk(sizeof(metadata));
    if(*metadata_pointer = -1){
        return nullptr;
    }
    (metadata)*metadata_pointer = {nullptr, tail, size, false, nullptr};
    if(!head){
        head = metadata_pointer;
        tail = metadata_pointer;
    }
    else {
        tail->next = metadata_pointer;
        tail = tail->next;
    }
    auto address = sbrk(size);
    if(*address = -1){
        return nullptr;
    }
    metadata_pointer->address = address;
    ++num_allocated_bolcks;
    num_allocated_bytes += size;
    return address;
}

void* calloc(size_t num, size_t size){
    auto address = malloc(num*size);
    if(!address){
        return nullptr;
    }
    std::memset(address, 0, num*size);
    return address;
}

void free(void* p){
    if(!p)
        return;
    metadata* iterator = head;
    while(!iterator){
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
    auto pointer = malloc(size);
    if(!pointer)
        return nullptr;
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
    return num_allocated_bolcks;
}

size_t _num_allocated_bytes(){
    return num_allocated_bytes;
}

size_t _num_meta_data_bytes(){
    return sizeof(metadata)*num_allocated_bolcks;
}

size_t _size_meta_data(){
    return sizeof(metadata);
}



