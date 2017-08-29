#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "sstl.h"

SSTL_NAMESPACE_BEGIN

template <class T1,class T2>
struct pai
{
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pai(first_type a,second_type b):first(a),second(b){}
    pai(const pai& p):first(p.first),second(p.second){}

};

void jiba(){}

SSTL_NAMESPACE_END

#endif // UTILITY_H_INCLUDED
