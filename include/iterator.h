#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

#include "sstl.h"

SSTL_NAMESPACE_BEGIN


//back_insert_iterator
template <class Container>
class back_insert_iterator
{
public:

    //typedefs
    typedef output_iterator_tag iterator_category;
    //disabled
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;


    //constructor
    explicit back_insert_iterator(Container& con):container(&con){}


    //assignment
    back_insert_iterator& operator=(const typename Container::value_type& value)
    {
        container->push_back(value);
        return *this;
    }

    //disabled
    back_insert_iterator& operator*(){return *this;}
    back_insert_iterator& operator++(){return *this;}
    back_insert_iterator& operator++(int){return *this;}

private:
    Container* container;

};


template <class Container>
back_insert_iterator<Container> back_inserter(Container& con)
{
    return back_insert_iterator<Container>(con);
}


//front_insert_iterator
template<class Container>
class front_insert_iterator
{
public:

    //typedefs
    typedef output_iterator_tag iterator_category;
    //disabled
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    typedef back_insert_iterator<Container> back_insert_iterator;


    //constructor
    explicit front_insert_iterator(Container& con):container(&con){}


    //assignment
    front_insert_iterator& operator=(const typename Container::value_type value)
    {
        container->push_front(value);
        return *this;
    }


    //disabled
    back_insert_iterator& operator*(){return *this;}
    back_insert_iterator& operator++(){return *this;}
    back_insert_iterator& operator++(int){return *this;}

private:
    Container* container;

};


template <class Container>
front_insert_iterator<Container> front_inserter(Container con)
{
    return front_insert_iterator<Container>(con);
}


//insert_iterator
template <class Container>
class insert_iterator
{
public:

    //typedefs
    typedef output_iterator_tag iterator_category;
    //disabled
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    typedef back_insert_iterator<Container> back_insert_iterator;

    //constructor
    insert_iterator(Container& con,typename Container::iterator i):container(&con),iter(i){}


    //assignment
    insert_iterator& operator=(const typename Container::value_type value)
    {
        container->insert(iter++,value);
        return *this;
    }


    //disabled
    back_insert_iterator& operator*(){return *this;}
    back_insert_iterator& operator++(){return *this;}
    back_insert_iterator& operator++(int){return *this;}

private:
    Container* container;
    typename Container::iterator iter;

};


template <class Container,class Iterator>
insert_iterator<Container> inserter(Container& con,Iterator i)
{
    typedef typename Container::iterator iter;
    return insert_iterator<Container>(con,iter(i));
}


//reverse_iterator
template <class Iterator>
class reverse_iterator
{
public:

    //typedefs
    typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
    typedef typename iterator_traits<Iterator>::value_type          value_type;
    typedef typename iterator_traits<Iterator>::difference_type     difference_type;
    typedef typename iterator_traits<Iterator>::pointer             pointer;
    typedef typename iterator_traits<Iterator>::reference           reference;

    typedef Iterator            iterator;


    //constructor
    reverse_iterator()=default;

    explicit reverse_iterator(iterator i):_current(i){}


    //get positives iterator
    iterator base() const {return _current;}


    //data access
    reference operator*() const
    {
        Iterator tmp=_current;
        return *--tmp;
    }

    pointer operator->() const {return &(operator*());}

    reference operator[](difference_type n) const {return *(*this+n);}


    //arithmetic
    reverse_iterator& operator++()
    {
        --_current;
        return *this;
    }

    reverse_iterator operator++(int)
    {
        reverse_iterator tmp=*this;
        --_current;
        return tmp;
    }

    reverse_iterator& operator--()
    {
        ++_current;
        return *this;
    }

    reverse_iterator operator--(int)
    {
        reverse_iterator tmp=*this;
        ++_current;
        return tmp;
    }

    reverse_iterator operator+(difference_type n) const
    {
        return reverse_iterator(_current-n);
    }

    reverse_iterator operator-(difference_type n) const
    {
        return reverse_iterator(_current+n);
    }

    reverse_iterator& operator+=(difference_type n)
    {
        _current-=n;
        return *this;
    }

    reverse_iterator& operator-=(difference_type n)
    {
        _current+=n;
        return *this;
    }


    //relational
    bool operator==(const reverse_iterator& riter) const {return _current==riter._current;}

    bool operator!=(const reverse_iterator& riter) const {return _current!=riter._current;}

private:

    Iterator _current;

};


//const_iterator
template <class Iterator>
class const_iterator
{
public:

    typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
    typedef typename iterator_traits<Iterator>::value_type          value_type;
    typedef typename iterator_traits<Iterator>::difference_type     difference_type;
    typedef const value_type*                                       pointer;
    typedef const value_type&                                       reference;

    typedef Iterator iterator;


    //constructors
    const_iterator()=default;

    explicit const_iterator(iterator iter):_current(iter){}


    //get non-const iterator
    iterator base() const {return _current;}


    //data_access
    reference operator*() const {return *_current;}

    pointer operator->() const {return &(operator*());}

    reference operator[](difference_type n) const {return *(_current+n);}


    //arithmetic operators
    const_iterator& operator++()
    {
        ++_current;
        return *this;
    }

    const_iterator& operator--()
    {
        --_current;
        return *this;
    }

    const_iterator operator++(int)
    {
        const_iterator tmp=*this;
        ++*this;
        return tmp;
    }

    const_iterator operator--(int)
    {
        const_iterator tmp=*this;
        --*this;
        return tmp;
    }

    const_iterator operator+(difference_type n) const
    {
        return const_iterator(_current+n);
    }

    const_iterator operator-(difference_type n) const
    {
        return const_iterator(_current-n);
    }

    const_iterator& operator+=(difference_type n)
    {
        _current+=n;
        return *this;
    }

    const_iterator& operator-=(difference_type n)
    {
        _current-=n;
        return *this;
    }


    //relational operators
    bool operator==(const const_iterator& citer) const {return _current==citer._current;}

    bool operator!=(const const_iterator& citer) const {return !(operator==(citer));}



private:

    Iterator _current;
};



SSTL_NAMESPACE_END


#endif // ITERATOR_H_INCLUDED
