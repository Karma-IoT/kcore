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

template <class I>
concept bool WeaklyIncrementable =
    Semiregular<I> &&
    requires(I i) {
        typename difference_type_t<I>;
        requires SignedIntegral<difference_type_t<I>>;
        { ++i } -> Same<I>&;
        i++;
    };

template <class I>
concept bool Iterator =
    requires(I i) {
        { *i } -> auto&&; // 要求： i 可解引用
    } &&
    WeaklyIncrementable<I>;

template <class T>
concept bool value_type_t = requires {
    typename T::value_type;
}

template <class T>
concept bool reference_t = requires {
    typename T::reference;
}

template <class T>
concept bool rvalue_reference_t = requires {
    typename T::rvalue_reference;
}

template <class T>
concept bool iterator_category_t = requires {
    typename T::iterator_category;
}

template < class In >
concept bool Readable =
    requires {
        typename value_type_t<In>;
        typename reference_t<In>;
        typename rvalue_reference_t<In>;
    } &&
    CommonReference<
        reference_t<In>&&, value_type_t<In>&
    > &&
    CommonReference<
        reference_t<In>&&, rvalue_reference_t<In>&&
    > &&
    CommonReference<
        rvalue_reference_t<In>&&, const value_type_t<In>&
    >;

struct input_iterator_tag { };

struct output_iterator_tag { };

struct forward_iterator_tag : public input_iterator_tag { };

struct bidirectional_iterator_tag : public forward_iterator_tag { };

struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template < class I >
concept bool InputIterator =
    Iterator<I> && Readable<I> &&
    requires { typename iterator_category_t<I>; } &&
    DerivedFrom<iterator_category_t<I>, input_iterator_tag>;

template <class I>
concept bool Incrementable =
    Regular<I> &&
    WeaklyIncrementable<I> &&
    requires(I i) {
        { i++ } -> Same<I>&&;
    };

template < class S, class I >
concept bool Sentinel =
    Semiregular<S> && Iterator<I> &&
    WeaklyEqualityComparableWith<S, I>;

template < class I >
concept bool ForwardIterator =
    InputIterator<I> &&
    DerivedFrom<iterator_category_t<I>, forward_iterator_tag> &&
    Incrementable<I> &&
    Sentinel<I, I>;

template < class I >
concept bool BidirectionalIterator =
    ForwardIterator<I> &&
    DerivedFrom<
        iterator_category_t<I>, bidirectional_iterator_tag
    > &&
    requires(I i) {
        { --i } -> Same<I>&;
        { i-- } -> Same<I>&&;
    };


template <class S, class I>
constexpr bool disable_sized_sentinel = false;

template <class S, class I>
concept bool SizedSentinel =
    Sentinel<S, I> &&
    !disable_sized_sentinel<remove_cv_t<S>, remove_cv_t<I>> &&
    requires(const I& i, const S& s) {
        { s - i } -> Same<difference_type_t<I>>&&;
        { i - s } -> Same<difference_type_t<I>>&&;
    };

template <class I>
concept bool RandomAccessIterator =
    BidirectionalIterator<I> &&
    DerivedFrom<
        iterator_category_t<I>, random_access_iterator_tag
    > &&
    StrictTotallyOrdered<I> &&
    SizedSentinel<I, I> &&
    requires(I i, const I j, const difference_type_t<I> n) {
        { i += n } -> Same<I>&;
        { j + n }  -> Same<I>&&;
        { n + j }  -> Same<I>&&;
        { i -= n } -> Same<I>&;
        { j - n }  -> Same<I>&&;
        j[n];
        requires Same<decltype(j[n]), reference_t<I>>;
    };

} /* KCORE_NAMESPACE */

#endif /* STL_ALTERNATIVE_ITERATOR_TRAITS_H_ */
