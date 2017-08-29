#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "sstl.h"
#include "allocator.h"
#include "iterator_traits.h"
#include "type_traits.h"
#include "iterator.h"

#include <initializer_list>

SSTL_NAMESPACE_BEGIN

template <class T>
struct list_node
{
    list_node *prev;
    list_node *next;
    T value;
};

template <class T>
struct list_iterator
{
    //typedefs

    typedef bidirectional_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T&                          reference;
    typedef T*                          pointer;
    typedef ptrdiff_t                   difference_type;

    typedef list_iterator               iterator;
    typedef list_node<T>*               link_type;


    //wrapped pointer
    link_type _node;


    //constructors
    list_iterator()=default;

    list_iterator(link_type x):_node(x){}

    list_iterator(const iterator& iter):_node(iter._node){}


    //data access
    reference operator*() const{return _node->value;}

    pointer operator->() const{return &(operator*());}


    //arithmetic operators
    iterator& operator++()
    {
        _node=_node->next;
        return *this;
    }

    iterator operator++(int)
    {
        iterator tmp=*this;
        ++*this;
        return tmp;
    }

    iterator& operator--()
    {
        _node=_node->prev;
        return *this;
    }

    iterator operator--(int)
    {
        iterator tmp=*this;
        --*this;
        return tmp;
    }


    //relational operators
    bool operator==(const iterator& iter) const{return _node=iter._node;}

    bool operator!=(const iterator& iter) const{return _node!=iter._node;}

};

template <class T,class Alloc=malloc_alloc>
class list
{
public:

    typedef T                           value_type;
    typedef list_iterator<T>            iterator;
    typedef reverse_iterator<iterator>  reverse_iterator;
    typedef size_t                      size_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef const T&                    const_reference;
    typedef ptrdiff_t                   difference_type;


    //constructors
    list(){empty_initialize();}

    explicit list(size_type n):list(){insert_fill(end(),n);}

    explicit list(size_type n,value_type value):list(){insert_fill(end(),n,value);}

    list(const list& li):list(){insert_range(end(),li.begin(),li.end());}

    list(list&& li):_node(li._node),_size(li._size){li._node=nullptr;}

    template <class InputIterator>
    list(InputIterator begin,InputIterator end):list()
    {
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        insert_dispatch(_node,begin,end,isInteger());       //name confliction
    }

    list(std::initializer_list<value_type> il):list(){insert_range(end(),il.begin(),il.end());}


    //iterators
    iterator begin() const{return _node->next;}

    iterator end() const{return _node;}

    reverse_iterator rbegin() const {return reverse_iterator(end());}

    reverse_iterator rend() const {return reverse_iterator(begin());}


    //assignment
    list& operator=(const list& li)
    {
        clear();
        insert_range(end(),li.begin(),li.end());
        return *this;
    }

    list& operator=(std::initializer_list<value_type> il)
    {
        clear();
        insert_range(end(),il.begin(),il.end());
        return *this;
    }


    //swap & assign
    void swap(list& li)
    {
        link_type tmp_node=li._node;li._node=_node;_node=tmp_node;
        size_type tmp_size=li._size;li._size=_size;_size=tmp_size;
    }

    template <class InputIterator>
    list& assign(InputIterator begin,InputIterator end)
    {
        clear();
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        insert_dispatch(_node,begin,end,isInteger());           //name confliction
        return *this;
    }

    list& assign(std::initializer_list<value_type> il)
    {
        return operator=(il);
    }

    list& assign(size_type n,value_type value)
    {
        clear();
        insert_fill(end(),n,value);
        return *this;
    }


    //size
    size_type size(){return _size;}

    bool empty() const{return _node->next==_node;}


    //data access
    reference front(){return *begin();}

    reference back(){return *(--end());}


    //insert
    void push_back(const_reference value){insert(end(),value);}

    void push_front(const_reference value){insert(begin(),value);}

    template <typename... Args>
    void emplace_back(const Args&... args){emplace(end(),args...);}

    template <typename... Args>
    void emplace_front(const Args&... args){emplace(begin(),args...);}

    iterator insert(iterator tar)
    {
        link_type tmp=create_node();
        tmp->next=tar._node;
        tmp->prev=tar._node->prev;
        tar._node->prev->next=tmp;
        tar._node->prev=tmp;
        ++_size;
        return tmp;
    }

    iterator insert(iterator tar,const_reference value)
    {
        link_type tmp=create_node(value);
        tmp->next=tar._node;
        tmp->prev=tar._node->prev;
        tar._node->prev->next=tmp;
        tar._node->prev=tmp;
        ++_size;
        return tmp;
    }

    template <typename... Args>
    iterator emplace(iterator tar,const Args&... args)
    {
        link_type tmp=create_node(args...);
        tmp->next=tar._node;
        tmp->prev=tar._node->prev;
        tar._node->prev->next=tmp;
        tar._node->prev=tmp;
        ++_size;
        return tmp;
    }

    iterator insert(iterator tar,size_type n,value_type value){return insert_fill(tar,n,value);}

    template <class InputIterator>
    iterator insert(iterator tar,InputIterator begin,InputIterator end)
    {
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        return insert_dispatch(tar,begin,end,isInteger());
    }

    iterator insert(iterator tar,std::initializer_list<value_type> il){return insert_range(tar,il.begin(),il.end());}


    //erase
    void pop_front(){erase(begin());}

    void pop_back(){iterator tmp=end();erase(--tmp);}

    iterator erase(iterator tar)
    {
        link_type next_node=tar._node->next;
        link_type prev_node=tar._node->prev;
        prev_node->next=next_node;
        next_node->prev=prev_node;
        destroy_node(tar._node);
        --_size;
        return next_node;
    }

    iterator erase(iterator begin,iterator end)
    {
        while(begin!=end)
            erase(begin++);
        return begin;
    }

    void clear()
    {
        link_type cur=_node->next;

        while(cur!=_node){
            link_type tmp=cur;
            cur=cur->next;
            destroy_node(tmp);

        }
        _node->next=_node;
        _node->prev=_node;
        _size=0;
    }

    //resize
    void resize(size_type n);

    void resize(size_type n,value_type value);


    //destructor
    ~list(){clear();put_node(_node);}


private:
    typedef list_node<T>                    list_node;
    typedef list_node*                      link_type;
    typedef simple_alloc<list_node,Alloc>   list_node_allocator;

    link_type _node;
    size_type _size=0;


    //allocate and deallocate
    link_type get_node(){return list_node_allocator::allocate();}

    void put_node(link_type p){list_node_allocator::deallocate(p);}


    //create and destroy
    link_type create_node()
    {
        link_type p=get_node();
        construct(&p->value);
        return p;
    }

    link_type create_node(const_reference value)
    {
        link_type p=get_node();
        construct(&p->value,value);
        return p;
    }

    template <typename... Args>
    link_type create_node(const Args&... args)
    {
        link_type p=get_node();
        construct(&p->value,args...);
        return p;
    }

    void destroy_node(link_type p)
    {
        destroy(&p->value);
        put_node(p);
    }


    //initialize
    void empty_initialize()
    {
        _node=get_node();
        _node->next=_node;
        _node->prev=_node;
    }


    //insert aux
    template <class InputIterator>
    iterator insert_dispatch(iterator tar,InputIterator begin,InputIterator end,false_type)
    {
        return insert_range(tar,begin,end);
    }

    template <typename U>
    iterator insert_dispatch(iterator tar,U size,U value,true_type)
    {
        return insert_fill(tar,size,value);
    }

    template <class InputIterator>
    iterator insert_range(iterator tar,InputIterator begin,InputIterator end)
    {
        iterator ret;
        ret=insert(tar,*begin++);
        while(begin!=end)
            insert(tar,*begin++);
        return ret;
    }

    iterator insert_fill(iterator tar,size_type n)
    {
        iterator ret;
        ret=insert(tar);
        while(--n)
            insert(tar);
        return ret;
    }

    iterator insert_fill(iterator tar,size_type n,value_type value)
    {
        iterator ret;
        ret=insert(tar,value);
        while(--n)
            insert(tar,value);
        return ret;
    }

};

SSTL_NAMESPACE_END


#endif // LIST_H_INCLUDED
