#ifndef NUMERIC_H_INCLUDED
#define NUMERIC_H_INCLUDED

#include "sstl.h"

SSTL_NAMESPACE_BEGIN

//accumulate
template <class Iterator,class T>
T accumulate(Iterator begin,Iterator end,T sum)
{
    while(begin!=end)
        sum=sum+*begin++;
    return sum;
}


//adjacent_difference
template <class InputIterator,class OutputIterator>
OutputIterator adjacent_difference(InputIterator begin,
                                   InputIterator end,
                                   OutputIterator result)
{
    typedef typename iterator_traits<InputIterator>::value_type value_type;
    if(begin==end) return result;
    *result=*begin;
    value_type value=*begin;
    while(++begin!=end){
        value_type tmp=*begin;
        *++result=tmp-value;
        value=tmp;
    }
    return ++result;
}


//inner_product
template <class InputIterator1,class InputIterator2,class T>
T inner_product(InputIterator1 begin1,InputIterator1 end1,
                InputIterator2 begin2,T sum)
{
    for(;begin1!=end1;++begin1,++begin2)
    {
        sum=sum+(*begin1)*(*begin2);
        std::cout << sum << " ";
    }

    return sum;
}


//patial_sum
template <class InputIterator,class OutputIterator>
OutputIterator partial_sum(InputIterator begin,InputIterator end,
                           OutputIterator result)
{
    typedef typename iterator_traits<InputIterator>::value_type value_type;
    *result=*begin;
    value_type value=*begin;
    while(++begin!=end){
        value=value+*begin;
        *++result=value;
    }
    return ++result;
}




SSTL_NAMESPACE_END


#endif // NUMERIC_H_INCLUDED
