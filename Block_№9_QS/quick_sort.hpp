#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include "Exception.hpp"


template <typename Iter>
concept RandomAccessIterator = std::bidirectional_iterator<Iter>
&& requires (const Iter i, const Iter j, Iter k, const int n)
{
    { i + n } -> std::same_as<Iter>;
    { n + i } -> std::same_as<Iter>;
    { i - n } -> std::same_as<Iter>;
    {  i[n]  } -> std::same_as<std::iter_reference_t<Iter>>;

    { k += n } -> std::same_as<Iter&>;
    { k -= n } -> std::same_as<Iter&>;

    { i < j } -> std::convertible_to<bool>;
 
    std::iter_swap(i, j);
    *i;
};

template <typename Comp, typename Iter>
concept Comparator = requires (const typename std::iterator_traits <Iter>::value_type& i, const typename std::iterator_traits <Iter>::value_type& j, Comp foo)
{
    { foo(i,j) } -> std::same_as <bool>;
};


template<RandomAccessIterator RanAccIter, Comparator<RanAccIter> Compare>
void quick_sort(RanAccIter first, RanAccIter last, Compare comp)
{
    if (first > last)
    {
        throw Invalid_value_exception();
    }

    size_t size = std::distance(first, last);

    if (size < 2)
    {
        return;
    }


    RanAccIter head = first;
    RanAccIter tail = std::prev(last);
    auto pivot = first[size / 2];

    while (true)
    {
        while (comp(*head, pivot))
            ++head;
        while (comp(pivot, *tail))
            --tail;
        if (head >= tail)
        {
            break;
        }
        std::iter_swap(head++, tail--);
    }

    quick_sort(first, head, comp);
    quick_sort(head, last, comp);
}

template<RandomAccessIterator RanAccIter>
void quick_sort(RanAccIter first, RanAccIter last)
{
    quick_sort(first, last, std::less< typename std::iterator_traits< RanAccIter >::value_type >());
}