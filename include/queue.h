#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "deque.h"
#include "vector.h"
#include "algorithm.h"

#include <initializer_list>

SSTL_NAMESPACE_BEGIN

template <class T,class Sequence= deque<T>>
class queue
{
public:

    //typedefs
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;


    //size
    bool empty() const{return _con.empty();}

    size_type size() const{return _con.size();}


    //data access
    reference front(){return _con.front();}

    const_reference front() const{return _con.front();}

    reference back(){return _con.back();}

    const_reference back() const{return _con.back();}


    //push and pop
    void push(const_reference value){_con.push_back(value);}

    void pop(){_con.pop_front();}

private:

    Sequence _con;

};


template <class T,class Sequence=vector<T>>
class priority_queue
{
public:

    //typedefs
    typedef typename Sequence::value_type       value_type;
    typedef typename Sequence::size_type        size_type;
    typedef typename Sequence::reference        reference;
    typedef typename Sequence::const_reference  const_reference;


    //_constructors
    priority_queue():_con(){}

    template <class InputIterator>
    priority_queue(InputIterator begin,InputIterator end):_con(begin,end)
    {
        make_heap(_con.begin(),_con.end());
    }

    priority_queue(std::initializer_list<value_type> il):_con(il)
    {
        make_heap(_con.begin(),_con.end());
    }


    //size
    bool empty() const{return _con.empty();}

    size_type size() const{return _con.size();}


    //data access
    const_reference top() const{return _con.front();}


    //push and pop
    void push(const_reference value)
    {
        _con.push_back(value);
        push_heap(_con.begin(),_con.end());
    }

    void pop()
    {
        pop_heap(_con.begin(),_con.end());
        _con.pop_back();
    }

private:

    Sequence _con;

};


SSTL_NAMESPACE_END

#endif // QUEUE_H_INCLUDED
