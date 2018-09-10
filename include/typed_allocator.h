/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Author: tiannian <dtiannian@aliyun.com>
 *
 */

/**
 * \file
 *         Typed memory allocator.
 * \author
 *         tiannian <dtiannian@aliyun.com>
 *
 */

#ifndef TYPED_ALLOCATOR_H_
#define TYPED_ALLOCATOR_H_

#include "constants.h"
#include "kstring.h"

KCORE_NAMESPACE_BEGIN

template<class Tp, int N>
class TypedAllocator {
    public:
        TypedAllocator();
        TypedAllocator(const TypedAllocator &) = delete;
        TypedAllocator(const TypedAllocator &&) = delete;
        ~TypedAllocator() = default;
        
        auto alloc() -> Tp *;
        
        auto free(Tp *ptr) -> char;
        
        auto has_object(Tp *ptr) -> bool;
        
        auto empty_num() -> int;
        
    private:
        Tp mem[N];
        char count[N];
};


template<class Tp, int N>
TypedAllocator<Tp,N>::TypedAllocator() {
    memset(this->count, 0, N);
    memset(this->mem, 0, sizeof(Tp) * N);
}

template<class Tp, int N>
auto TypedAllocator<Tp,N>::alloc() -> Tp * {
    int i;
    for(i = 0; i < N; ++i) {
        if(this->count[i] == 0) {
            /* If this block was unused, we increase the reference count to
            indicate that it now is used and return a pointer to the
            memory block. */
            ++(this->count[i]);
            return (Tp *)((char *)this->mem + (i * sizeof(Tp)));
        }
    }

    /* No free block was found, so we return NULL to indicate failure to
     allocate block. */
    return nullptr;
}

template<class Tp, int N>
auto TypedAllocator<Tp,N>::free(Tp *ptr) -> char {
    int i;
    char *ptr2;

    /* Walk through the list of blocks and try to find the block to
    which the pointer "ptr" points to. */
    ptr2 = (char *)this->mem;
    for(i = 0; i < N; ++i) {

        if(ptr2 == (char *)ptr) {
            /* We've found to block to which "ptr" points so we decrease the
            reference count and return the new value of it. */
            if(this->count[i] > 0) {
                /* Make sure that we don't deallocate free memory. */
                --(this->count[i]);
            }
            return this->count[i];
        }
        ptr2 += this->size;
    }
    return -1;
}

template<class Tp, int N>
auto TypedAllocator<Tp,N>::has_object(Tp *ptr) -> bool{
    return (char *)ptr >= (char *)this->mem &&
        (char *)ptr < (char *)this->mem + (N * sizeof(Tp));
}

template<class Tp, int N>
auto TypedAllocator<Tp,N>::empty_num() -> int{
    int i;
    int num_free = 0;

    for(i = 0; i < N; ++i) {
        if(this->count[i] == 0) {
            ++num_free;
        }
    }
    
    return num_free;
}

KCORE_NAMESPACE_END

#endif /* TYPED_ALLOCATOR_H_ */
