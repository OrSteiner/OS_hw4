//
// Created by orsht on 6/17/2019.
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


void* unite(metadata* data1, metadata* data2){
    data1->size += data2->size + sizeof(metadata);
    data1->next = data2->next;
    --num_free_blocks;
    --num_allocated_blocks;
    num_free_bytes += sizeof(metadata);
    num_allocated_bytes += sizeof(metadata);
    return data1;
}

void* split(metadata* iterator, size_t size){
    size_t old_size = iterator->size;
    iterator->size = size;
    metadata* data = (metadata*)(iterator->address + size);
    data->next = iterator->next;
    data->prev = iterator;
    data->size = old_size - size - sizeof(metadata);
    data->is_free = true;
    data->address = iterator->address + size + sizeof(metadata);
    iterator->next = data;
    ++num_allocated_blocks;
    num_allocated_bytes -= sizeof(metadata);
    ++num_free_blocks;
    num_free_bytes -= sizeof(metadata);
    if(iterator->next->is_free){
        unite(iterator, iterator->next);
    }
    return iterator->address;
}

void* malloc(size_t size){
    if(size == 0 || size > 100000000){
        return NULL;
    }
    metadata* iterator = head;
    while(iterator){
        if(iterator->is_free && iterator->size > size){
            if(iterator->size - size - sizeof(metadata) >= 128){
                iterator->address = split(iterator, size);
            }
            iterator->is_free = false;
            --num_free_blocks;
            num_free_bytes -= iterator->size;
            return iterator->address;
        }
        iterator = iterator->next;
    }
    if(tail->is_free){ //extending the last block for the new size.
        size_t extension = size - tail->size;
        void* temp = sbrk(extension);
        if(*((int*)temp) == -1){
            return NULL;
        }
        tail->size = size;

    }

    metadata* metadata_pointer = (metadata*)sbrk((sizeof(metadata) + (4-(sizeof(metadata)%4))%4));

    if(*((int*)metadata_pointer) == -1){
        return NULL;
    }
    metadata_pointer->next= NULL;
    metadata_pointer->prev=tail;
    metadata_pointer->size=size;
    metadata_pointer->is_free=false;
    metadata_pointer->address= NULL;
    if(!head){
        head = metadata_pointer;
        tail = metadata_pointer;
    }
    else {
        tail->next = metadata_pointer;
        tail = tail->next;
    }
    void* address = sbrk(size + (4-(size%4)%4));
    if(*((int*)address) == -1){
        return NULL;
    }
    metadata_pointer->address = address;
    ++num_allocated_bolcks;
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
            if(iterator->next->is_free){
                iterattor = unite(iterator, iterator->next);
            }
            if(iterator->prev->is_free){
                iterator = unite(iterator->prev, iterator);
            }
            return;
        }
        iterator = iterator->next;
    }
}

void* realloc(void* oldp, size_t size){
    metadata* iterator = head;
    while(iterator){
        if(iterator->address == oldp){
            if(size <= iterator->size){
                if(iterator->size - size - sizeof(metadata) >= 128){
                    return split(iterator, size);
                }
                else
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



