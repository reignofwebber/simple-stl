#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "deque.h"

SSTL_NAMESPACE_BEGIN

template <T,class Sequence=deque<T>>
class stack
{
public:

    //typedefs
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;]
    typedef typename Sequence::const_reference const_reference;


    //size
    bool empty() const{return con.empty();}

    size_type size() const{return con.size();}


    //data access
    reference top(){return con.back();}

    const_reference top() const{return con.back();}


    //push and pop
    void push(const_reference value){con.push_back(value);}

    void pop(){con.pop_back();}


private:

    Sequence con;
};

SSTL_NAMESPACE_END

#endif // STACK_H_INCLUDED
