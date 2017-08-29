#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "allocator.h"
#include "type_traits.h"
#include "iterator_traits.h"
#include "iterator.h"

#include <initializer_list>


SSTL_NAMESPACE_BEGIN

template <class T,class Alloc=malloc_alloc>
class vector
{
public:

    //typedefs
    typedef T                           value_type;
    typedef T*                          iterator;
    typedef const T*                    const_iterator;
    typedef reverse_iterator<iterator>  reverse_iterator;
    typedef size_t                      size_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef const T&                    const_reference;
    typedef ptrdiff_t                   difference_type;


    //constructors
    vector(){initialize_n();}

    explicit vector(size_type n){initialize_n(n);}

    explicit vector(size_type n,value_type value):vector(){insert_fill(begin(),value,n);}

    vector(const vector &v):vector(){insert_range(begin(),v.begin(),v.end());}

    vector(vector&& v):_begin(v._begin),_end(v._end),_cap(v._cap){_begin=_end=_cap=nullptr;}

    template <class InputIterator>
    vector(InputIterator begin,InputIterator end):vector()
    {
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        insert_dispatch(_begin,begin,end,isInteger());
    }

    vector(std::initializer_list<value_type> il):vector(){insert_range(begin(),il.begin(),il.end());}


    //iterators
    iterator begin() const{return _begin;}

    iterator end() const{return _end;}

    const_iterator cbegin() const{return _begin;}

    const_iterator cend() const{return _end;}

    reverse_iterator rbegin() const {return reverse_iterator(_end);}

    reverse_iterator rend() const {return reverse_iterator(_begin);}


    //assignment
    vector& operator=(const vector &v)
    {
        free();
        insert_range(begin(),v.begin(),v.end());
        return *this;
    }

    vector& operator=(std::initializer_list<value_type> il)
    {
        free();
        insert_range(begin(),il.begin(),il.end());
        return *this;
    }


    //swap & assign
    void swap(vector &v)
    {
        auto tmp_begin=_begin;auto tmp_end=_end;auto tmp_cap=_cap;
        _begin=v._begin;_end=v._end;_cap=v._cap;
        v._begin=tmp_begin;v._end=tmp_end;v._cap=tmp_cap;
    }

    template <class InputIterator>
    vector& assign(InputIterator begin,InputIterator end)
    {
        free();
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        insert_dispatch(_begin,begin,end,isInteger());
        return *this;
    }

    vector& assign(std::initializer_list<value_type> il){return operator=(il);}

    vector& assign(size_type n,value_type value)
    {
        free();
        insert_fill(begin(),value,n);
        return *this;
    }


    //size
    size_type size() const{return _end-_begin;}

    bool empty() const{return _begin==_end;}


    //data access
    const_reference back() const{return *(end()-1);}

    const_reference front() const{return *begin();}

    reference operator[](size_type n){return *(begin()+n);}

    const_reference at(size_type n) const{return *(begin()+n);}   //to be perfected


    //insert
    void push_back(const_reference value){insert_fill(end(),value);}

    iterator insert(iterator tar,const_reference value){return insert_fill(tar,value);}

    template <typename... Args>
    void emplace_back(const Args... args){emplace(end(),args...);}

    template <typename... Args>
    iterator emplace(iterator tar,const Args&... args){return emplace_aux(tar,args...);}

    iterator insert(iterator tar,size_type n,const_reference value){return insert_fill(tar,value,n);}

    template <class InputIterator>
    iterator insert(iterator tar,InputIterator begin,InputIterator end)
    {
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        return insert_dispatch(tar,begin,end,isInteger());
    }

    iterator insert(iterator tar,std::initializer_list<T> il){return insert_range(tar,il.begin(),il.end());}


    //erase
    void pop_back(){erase_n(end()-1);}

    iterator erase(iterator tar){return erase_n(tar);}

    iterator erase(iterator tar,iterator end){return erase_n(tar,end-tar);}

    void clear(){destroy(_begin,_end);_end=_begin;}


    //resize
    void resize(size_type n)
    {
        if(n<size()){
            iterator tar=_begin+n;
            destroy(tar,_end);
            _end=tar;
        }else{
            chk_realloc(n);
            size_type extra=n-size();
            uninitialized_fill_n(_end,extra);
            _end+=extra;
        }
    }

    void resize(size_type n,value_type value)
    {
        if(n<size()){
            iterator tar=_begin+n;
            destroy(tar,_end);
            _end=tar;
        }else{
            chk_realloc(n);
            size_type extra=n-size();
            uninitialized_fill_n(_end,extra,value);
            _end+=extra;
        }
    }

    void shrink_to_fit(){realloc(size());}


    //capacity
    size_type capacity(){return _cap-_begin;}

    void reserve(size_type n){if(n>capacity()) realloc(n);}


    //destructor
    ~vector(){free();}


private:
    typedef simple_alloc<value_type,Alloc> data_allocator;

    iterator _begin;
    iterator _end;
    iterator _cap;


    //initialize
    void initialize_n(size_type n=0)
    {
        _begin=data_allocator::allocate(n);
        _cap=_end=_begin+n;

        uninitialized_fill(_begin,_end);
    }

    //memory related
    void free()
    {
        clear();
        data_allocator::deallocate(_begin);
        _begin=_end=_cap=nullptr;
    }

    void realloc(size_type cap)
    {
        auto tmp_begin=data_allocator::allocate(cap);
        auto tmp_end=uninitialized_copy(_begin,_end,tmp_begin);
        free();
        _begin=tmp_begin;_end=tmp_end;_cap=_begin+cap;
    }

    void chk_realloc(size_type n=1)
    {
        auto new_capacity=capacity();
        if(!size())
            new_capacity=1;
        while(size()+n>new_capacity)
            new_capacity*=2;

        if(new_capacity!=capacity())
            realloc(new_capacity);
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
        return insert_fill(tar,value,size);
    }

    template <typename... Args>
    iterator emplace_aux(iterator tar,const Args&... args)
    {
        difference_type copy_size=_end-tar;

        chk_realloc();
        iterator ret=_end++;
        while(copy_size--){
            --ret;
            construct(ret+1,*ret);
            destroy(ret);
        }
        construct(ret,args...);
        return ret;
    }

    iterator insert_fill(iterator tar,value_type value,size_type n=1)
    {
        difference_type copy_size=_end-tar;

        chk_realloc(n);
        iterator ret=_end;
        while(copy_size--){
            --ret;
            construct(ret+n,*ret);
            destroy(ret);
        }
        _end+=n;
        uninitialized_fill_n(ret,n,value);
        return ret;
    }

    template <class InputIterator>
    iterator insert_range(iterator tar,InputIterator begin,InputIterator end)
    {
        size_type n=sstl::distance(begin,end);
        difference_type copy_size=_end-tar;

        chk_realloc(n);
        iterator ret=_end;
        while(copy_size--){
            --ret;
            construct(ret+n,*ret);
            destroy(ret);
        }
        _end+=n;
        uninitialized_copy(begin,end,ret);
        return ret;
    }


    //erase aux
    iterator erase_n(iterator tar,size_type n=1)
    {
        destroy(tar,n);
        for(iterator iter=tar+n;iter!=_end;++iter){
            construct(iter-n,*iter);
            destroy(iter);
        }
        _end-=n;
        return tar;
    }

};


SSTL_NAMESPACE_END

#endif // VECTOR_H_INCLUDED
