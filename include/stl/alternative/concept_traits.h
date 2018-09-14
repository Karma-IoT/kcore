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
#include "type_traits"
#include "utility"

namespace KCORE_NAMESPACE {

template < class T, class U >
concept bool Same = is_same<T, U>::value;

template < class T >
concept bool Destructible = is_destructible<T>::value;

template <class From, class To>
concept bool ConvertibleTo =
    is_convertible<From, To>::value &&
    requires(From (&f)()) {
        static_cast<To>(f());
    };

template < class T, class... Args >
concept bool Constructible =
    Destructible<T> && is_constructible<T, Args...>::value;

template < class T >
concept bool DefaultConstructible = Constructible<T>;

template < class T >
concept bool MoveConstructible =
    Constructible<T, T> && ConvertibleTo<T, T>;

template <class T>
concept bool CopyConstructible =
    MoveConstructible<T> &&
    Constructible<T, T&> && ConvertibleTo<T&, T> &&
    Constructible<T, const T&> && ConvertibleTo<const T&, T> &&
    Constructible<T, const T> && ConvertibleTo<const T, T>;


template < class T, class U >
concept bool CommonReference =
    Same<common_reference_t<T, U>, common_reference_t<U, T>> &&
    ConvertibleTo<T, common_reference_t<T, U>> &&
    ConvertibleTo<U, common_reference_t<T, U>>;

template < class T, class U >
concept bool Assignable =
    is_lvalue_reference<T>::value &&
    CommonReference<
        const remove_reference_t<T>&,
        const remove_reference_t<U>&> &&
    requires(T t, U&& u) {
        { t = forward<U>(u) } -> Same<T>&&;
    };

template< class T >
concept bool Swappable =
    requires(T& a, T& b) {
        swap(a, b);
    };

template< class T, class U >
concept bool SwappableWith =
    CommonReference<
        const remove_reference_t<T>&,
        const remove_reference_t<U>&> &&
    requires(T&& t, U&& u) {
        swap(forward<T>(t), forward<T>(t));
        swap(forward<U>(u), forward<U>(u));
        swap(forward<T>(t), forward<U>(u));
        swap(forward<U>(u), forward<T>(t));
  };

template < class T >
concept bool Movable =
    is_object<T>::value &&
    MoveConstructible<T> &&
    Assignable<T&, T> &&
    Swappable<T>;

template <class T>
concept bool Copyable =
    CopyConstructible<T> &&
    Movable<T> &&
    Assignable<T&, const T&>;

template <class T>
concept bool Semiregular =
    Copyable<T> &&
    DefaultConstructible<T>;

template < class T >
concept bool Integral =
    is_integral<T>::value;

template < class T >
concept bool SignedIntegral =
    Integral<T> &&
    is_signed<T>::value;

template <class B>

concept bool Boolean =
    Movable<decay_t<B>> &&
    requires(const remove_reference_t<B>& b1,
             const remove_reference_t<B>& b2, const bool a) {
        { b1 }       -> ConvertibleTo<bool>&&;
        { !b1 }      -> ConvertibleTo<bool>&&;
        { b1 && a }  -> Same<bool>&&;
        { b1 || a }  -> Same<bool>&&;
        { b1 && b2 } -> Same<bool>&&;
        { a && b2  } -> Same<bool>&&;
        { b1 || b2 } -> Same<bool>&&;
        { a || b2  } -> Same<bool>&&;
        { b1 == b2 } -> ConvertibleTo<bool>&&;
        { b1 == a  } -> ConvertibleTo<bool>&&;
        { a == b2  } -> ConvertibleTo<bool>&&;
        { b1 != b2 } -> ConvertibleTo<bool>&&;
        { b1 != a  } -> ConvertibleTo<bool>&&;
        { a != b2  } -> ConvertibleTo<bool>&&;
    };

template <class T, class U>
concept bool WeaklyEqualityComparableWith =
    requires(const remove_reference_t<T>& t,
             const remove_reference_t<U>& u) {
        { t == u } -> Boolean&&;
        { t != u } -> Boolean&&;
        { u == t } -> Boolean&&;
        { u != t } -> Boolean&&;
    };

template < class T >
concept bool EqualityComparable = WeaklyEqualityComparableWith<T, T>;

template <class T, class U>
concept bool EqualityComparableWith =
    EqualityComparable<T> &&
    EqualityComparable<U> &&
    CommonReference<
        const remove_reference_t<T>&,
        const remove_reference_t<U>&> &&
    EqualityComparable<
        common_reference_t<
            const remove_reference_t<T>&,
            const remove_reference_t<U>&>> &&
    WeaklyEqualityComparableWith<T, U>;

template < class T, class U >
concept bool DerivedFrom =
    is_base_of<U, T>::value &&
    is_convertible<remove_cv_t<T>*, remove_cv_t<U>*>::value;

template <class T>
concept bool Regular =
    Semiregular<T> &&
    EqualityComparable<T>;

template <class T>
concept bool StrictTotallyOrdered =
    EqualityComparable<T> &&
    requires(const remove_reference_t<T>& a,
             const remove_reference_t<T>& b) {
        { a < b }  -> Boolean&&;
        { a > b }  -> Boolean&&;
        { a <= b } -> Boolean&&;
        { a >= b } -> Boolean&&;
    };

template <class T, class U>
concept bool StrictTotallyOrderedWith =
    StrictTotallyOrdered<T> &&
    StrictTotallyOrdered<U> &&
    CommonReference<
        const remove_reference_t<T>&,
        const remove_reference_t<U>&> &&
    StrictTotallyOrdered<
    common_reference_t<
        const remove_reference_t<T>&,
        const remove_reference_t<U>&>> &&
    EqualityComparableWith<T, U> &&
    requires(const remove_reference_t<T>& t,
             const remove_reference_t<U>& u) {
        { t < u }  -> Boolean&&;
        { t > u }  -> Boolean&&;
        { t <= u } -> Boolean&&;
        { t >= u } -> Boolean&&;
        { u < t }  -> Boolean&&;
        { u > t }  -> Boolean&&;
        { u <= t } -> Boolean&&;
        { u >= t } -> Boolean&&;
    };

} /* KCORE_NAMESPACE */

#endif /* STL_ALTERNATIVE_CONCEPT_TRAITS_H_ */
