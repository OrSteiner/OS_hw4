//
// Created by orsht on 6/16/2019.
//

#include <unistd.h>

void* mallic(size_t size){
    if(size == 0 || size > 100000000){
        return nullptr;
    }
    auto pointer = sbrk(size);
    if(*pointer = -1){
        return nullptr;
    }
    return pointer;
}