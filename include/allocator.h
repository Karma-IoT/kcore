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
 *         Concepts of allocator.
 * \author
 *         tiannian <dtiannian@aliyun.com>
 *
 */

#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "constants.h"
#include "stl/kstddef.h"

KCORE_NAMESPACE_BEGIN

KCORE_INNER_NAMESPACE_BEGIN

template<class T>
concept bool allocate = requires(T a,size_t n) {
    { a.allocate(n) } -> typename T::value_type*;
    { allocate(a,n) } -> typename T::value_type*;
}

template<class T>
concept bool allocate_with_hint = requires(T a, size_t n, const void *hint) {
    { a.allocate(n,hint) } -> typename T::value_type*;
    { allocate(a,n,hint) } -> typename T::value_type*;
}

template<class T>
concept bool deallocate = requires(T a, T *p, size_t n,) {
    { a.deallocate(p,n) } -> void;
    { deallocate(a,p,n) } -> void;
}

template<class T>

KCORE_INNER_NAMESPACE_END

template<class T>
concept bool Allocator = requires(T a) {
    typename T::size_type;
    typename T::difference_type;
} && requires (T a, size_t n) {
    { a.allocate(n) } -> typename T::value_type*;
} && requires (T a, size_t n, const void *hint) {
    { a.allocate(n,hint) } -> typename T::value_type *;
} && requires (T a, T *p,size_t n) {
    { a.deallocate(p,n) } -> void;
} && requires (T a, T b) {
    { a == b } -> bool;
    { a != b } -> bool;
};

template<class T>
concept bool 


KCORE_NAMESPACE_END
 
#endif /* ALLOCATOR_H_ */
