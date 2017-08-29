#ifndef TYPE_TRAITS_H_INCLUDED
#define TYPE_TRAITS_H_INCLUDED

#include "sstl.h"


SSTL_NAMESPACE_BEGIN

class true_type{};
class false_type{};

template <typename T>
struct type_traits
{
    typedef false_type isInteger;
};

template <>
struct type_traits<char>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<unsigned char>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<signed char>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<short>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<unsigned short>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<int>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<unsigned int>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<long>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<unsigned long>
{
    typedef true_type isInteger;
};


template <>
struct type_traits<long long>
{
    typedef true_type isInteger;
};

template <>
struct type_traits<unsigned long long>
{
    typedef true_type isInteger;
};





SSTL_NAMESPACE_END


#endif // TYPE_TRAITS_H_INCLUDED
