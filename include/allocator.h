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

namespace KCORE_NAMESPACE {

namespace KCORE_INNER_NAMESPACE {

template<class T>
concept bool base_allocator = requires(T a) {
    typename T::value_type;
} && requires (T a,T b) {
    { a == b } -> bool;
    { a != b } -> bool;
};

template<class T>
concept bool array_allocate = requires(T a,size_t n) {
    { a.allocate(n) } -> typename T::value_type*;
    // { T::allocate(a,n) } -> typename T::value_type*;
};

template<class T>
concept bool array_allocate_with_hint = requires(T a, size_t n, const void *hint) {
    { a.allocate(n,hint) } -> typename T::value_type*;
    // { T::allocate(a,n,hint) } -> typename T::value_type*;
};

template<class T>
concept bool array_deallocate = requires(T a, typename T::value_type *p, size_t n) {
    { a.deallocate(p,n) } -> void;
    // { T::deallocate(a,p,n) } -> void;
};

template<class T>
concept bool allocate = requires(T a) {
    { a.allocate() } -> typename T::value_type*;
    // { T::allocate(a) } -> typename T::value_type*;
};

template<class T>
concept bool deallocate = requires(T a, typename T::value_type *p) {
    { a.deallocate(p) } -> size_t;
    // { T::deallocate(a,p) } -> void;
};

} /* KCORE_INNER_NAMESPACE */

template<class T>
concept bool allocator = 
    KCORE_INNER_NAMESPACE::allocate<T> &&
    KCORE_INNER_NAMESPACE::deallocate<T> &&
    KCORE_INNER_NAMESPACE::base_allocator<T>;

template<class T>
concept bool array_allocator = 
    KCORE_INNER_NAMESPACE::array_allocate<T> &&
    KCORE_INNER_NAMESPACE::array_deallocate<T> &&
    KCORE_INNER_NAMESPACE::base_allocator<T>;

template<class T>
concept bool extend_allocator = 
    KCORE_INNER_NAMESPACE::base_allocator<T> &&
    requires (T a) {
        { a.available_count() } -> size_t;
    } && requires (T a, typename T::value_type *b) {
        { a.has(b) } -> bool;
    };

} /* KCORE_NAMESPACE */
 
#endif /* ALLOCATOR_H_ */
