#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

//strange error "utility.h"
#include "sstl.h"

#include <initializer_list>

SSTL_NAMESPACE_BEGIN

template <class T1,class T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    first_type first;
    second_type second;

    //constructors
    pair()=default;

    pair(first_type a,second_type b):first(a),second(b){}

    pair(const pair& p):first(p.first),second(p.second){}

    //relational operators
    bool operator==(const pair& p){return first==p.first && second==p.second;}

    bool operator!=(const pair& p){return !(operator==(p));}

};

template <class T1,class T2>
pair<T1,T2> make_pair(T1 x,T2 y)
{
    return pair<T1,T2>(x,y);
}

template <class T1,class T2>
std::ostream& operator<<(std::ostream& os,pair<T1,T2> p)
{
    os << "{" << p.first << "," << p.second << "}";
    return os;
}


SSTL_NAMESPACE_END

#endif // UTILITIES_H_INCLUDED
