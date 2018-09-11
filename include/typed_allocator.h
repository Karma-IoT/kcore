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
#include "stl/kstring.h"
#include "stl/kstddef.h"

namespace KCORE_NAMESPACE {

template<class Tp, int N> 
class typed_allocator {
    public:
        typed_allocator() {
            memset(this->count, 0, N);
            memset(this->mem, 0, sizeof(Tp) * N);
        }
        typed_allocator(const typed_allocator &) = delete;
        typed_allocator(const typed_allocator &&) = delete;
        ~typed_allocator() = default;
    public:
        using value_type = Tp;
    public:
        // impl allocator
        auto allocate() -> Tp*;
        auto deallocate(Tp *p) -> int;
    public:
        // impl extend_allocator
        auto available_count() -> size_t;
        auto has(Tp *) -> bool;
        
    private:
        Tp mem[N];
        char count[N];
};

template< class T1, int N1, class T2, int N2>
bool operator==( const typed_allocator<T1, N1>& lhs, const typed_allocator<T2, N2>& rhs );

template< class T1, int N1, class T2, int N2>
bool operator!=( const typed_allocator<T1, N1>& lhs, const typed_allocator<T2, N2>& rhs );

/*-------------------------------------------*/

template<class Tp, int N>
auto typed_allocator<Tp,N>::allocate() -> Tp * {
    for(int i = 0; i < N; ++i) {
        if(this->count[i] == 0) {
            ++(this->count[i]);
            return &(this->mem[i]);
        }
    }
    return nullptr;
}

template<class Tp, int N>
auto typed_allocator<Tp,N>::deallocate(Tp *p) -> int {
    if(this->has(p)){
        auto pos = (p - this->mem) / sizeof(Tp);
        if (this->count[pos] > 0) {
            -- (this -> count[pos]);
        }
        return this->count[pos];
    } 
    return -1;
}

template<class Tp, int N>
auto typed_allocator<Tp,N>::has(Tp *ptr) -> bool{
    return (char *)ptr >= (char *)this->mem &&
        (char *)ptr < (char *)this->mem + (N * sizeof(Tp));
}

template<class Tp, int N>
auto typed_allocator<Tp,N>::available_count() -> size_t{
    int num_free = 0;

    for(int i = 0; i < N; ++i) {
        if(this->count[i] == 0) {
            ++num_free;
        }
    }
    
    return num_free;
}

template< class T1, int N1, class T2, int N2>
bool operator==( const typed_allocator<T1, N1>& lhs, const typed_allocator<T2, N2>& rhs ) {
    return &lhs == &rhs;
}

template< class T1, int N1, class T2, int N2>
bool operator!=( const typed_allocator<T1, N1>& lhs, const typed_allocator<T2, N2>& rhs ) {
    return &lhs != &rhs;
}

}

#endif /* TYPED_ALLOCATOR_H_ */
