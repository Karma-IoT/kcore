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
 *         Concepts of iterator traits.
 * \author
 *         tiannian <dtiannian@aliyun.com>
 *
 */

#ifndef STL_ALTERNATIVE_ITERATOR_TRAITS_H_
#define STL_ALTERNATIVE_ITERATOR_TRAITS_H_

#include "constants.h"
#include "concept_traits.h"

namespace KCORE_NAMESPACE {
    
template<typename T>
concept bool Iterator = CopyConstructible<T> &&
    CopyAssignable<T> &&
    Destructible<T> &&
    Swappable<T> && 
    requires {
        typename T::value_type;
        typename T::difference_type;
        typename T::reference;
        typename T::pointer;
        typename T::iterator_category;
    } && requires(T r) {
        { *r };
        { ++r } -> T&;
    }
    

template<typename T>
concept bool InputIterator = Iterator<T> &&
    EqualityComparable<T> &&
    requires {
        typename T::reference;
        typename T::value_type;
    } && requires(T i, T j) {
        { i != j } -> bool;
        { *i } -> reference;
        { *i } -> value_type;
        { (void)i++ };
        { *i++ } -> value_type;
    };
    

template<typename T>
concept bool OutputIterator = Iterator<T> && 
    //is_class<T>::value &&
    //is_pointer<T>::value &&
    requires(T r, U o) {
        { *r = o };
        { *r++ = o };
        { ++r } -> T&;
        { r++ } -> const X&;
    }
    
    
template<typename T>
concept bool ForwardIterator = InputIterator<T> &&
    DefaultConstructible<T> &&
    // 多趟保证
    true;
    
} /* KCORE_NAMESPACE */
 
#endif /* STL_ALTERNATIVE_ITERATOR_TRAITS_H_ */
