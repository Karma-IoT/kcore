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

#ifndef STL_ALTERNATIVE_CONCEPT_TRAITS_H_
#define STL_ALTERNATIVE_CONCEPT_TRAITS_H_

#include "constants.h"

namespace KCORE_NAMESPACE {

template<typename T>
concept bool EqualityComparable = requires (T a, T b) {
    { a == b } -> bool;
}

template<typename T>
concept bool DefaultConstructible = requires {
    { T u } -> T;
    { T u{} } -> T;
    { T() } -> T;
    { T{} } -> T;
}

template<typename T>
concept bool MoveConstructible = requires (T rv) {
    // typename std::add_rvalue_reference<T>::type rv;
    { T u = rv } -> T;
    { T(rv) } -> T;
}

template<typename T>
concept bool CopyConstructible = MoveConstructible<T> && requires (T v) {
    // typename std::add_lvalue_reference<typename std::add_const<T>::type>::type v;
    { T u = v } -> T;
    { T(v) } -> T;
}

template<typename T>
concept bool MoveAssignable = requires (T rv) {
    // typename std::add_rvalue_reference<T>::type v;
    { t = rv } -> T&;
}

template<typename T>
concept bool CopyAssignable = MoveAssignable<T> && requires (T v) {
    // typename std::add_lvalue_reference<const T>::type v;
    { t = v } -> T&;
}

template<typename T>
concept bool Destructible = requires (T v) {
    // typename std::add_lvalue_reference<const T>::type v;
    { u.~T() };
}

template<typename T>
concept bool Swappable = true;

template<typename T>
concept bool NullablePointer = requires (T p, T q) {
    { p != q } -> bool;
} && requires (T p, nullptr_t np) {
    { T p(np) } -> nullptr_t;
    { T p = np } -> nullptr_t;
    { p = np } -> bool;
    { p == np } -> bool;
    { np == p } -> bool;
    { p != np } -> bool;
    { np != p } -> bool;
}

} /* KCORE_NAMESPACE */
 
#endif /* STL_ALTERNATIVE_CONCEPT_TRAITS_H_ */
