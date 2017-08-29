/*
    manipulate iterators
*/
#ifndef ITERATOR_TRAITS_H_INCLUDED
#define ITERATOR_TRAITS_H_INCLUDED

#include "sstl.h"

SSTL_NAMESPACE_BEGIN

//iterator_categories
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};


//standard iterator
template <class Category,
          class T,
          class Distance=ptrdiff_t,
          class Pointer=T*,
          class Reference=T&>
struct base_iterator
{
    typedef Category        iterator_category;
    typedef T               value_type;
    typedef Distance        difference_type;
    typedef Pointer         pointer;
    typedef Reference       reference;
};


//for generic algorithm
template <class Iterator>    //typename class
struct iterator_traits
{
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;
    typedef typename Iterator::difference_type      difference_type;
};


//specialize
template <typename T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef ptrdiff_t                   difference_type;
};

template <typename T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
    typedef ptrdiff_t                   difference_type;
};


//distance between two iterators
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator begin,InputIterator end)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return distance_dispatch(begin,end,category());
}

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator begin,InputIterator end,input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n=0;
    while(begin!=end){
        ++begin;++n;
    }
    return n;
}

template <class RandomAccessIterator>
typename iterator_traits<RandomAccessIterator>::difference_type
distance_dispatch(RandomAccessIterator begin,RandomAccessIterator end,random_access_iterator_tag)
{
    return end-begin;
}


//advance n
template <class InputIterator>
InputIterator advance(InputIterator iter,
                      typename iterator_traits<InputIterator>::difference_type n)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return advance_dispatch(iter,n,category());
}

template <class InputIterator>
InputIterator advance_dispatch(InputIterator iter,
                               typename iterator_traits<InputIterator>::difference_type n,
                               input_iterator_tag)
{
    if(n<0)
        while(n++)
            --iter;
    else
        while(n--)
            ++iter;
    return iter;
}

template <class RandomAccessIterator>
RandomAccessIterator advance_dispatch(RandomAccessIterator iter,
                                      typename iterator_traits<RandomAccessIterator>::difference_type n,
                                      random_access_iterator_tag)
{
    return iter+=n;
}


//extract Iterator's value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <class Iterator>
typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}


SSTL_NAMESPACE_END

#endif // ITERATOR_TRAITS_H_INCLUDED
