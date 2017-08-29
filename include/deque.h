#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

#include "sstl.h"
#include "allocator.h"
#include "type_traits.h"
#include "iterator.h"

#include <initializer_list>

SSTL_NAMESPACE_BEGIN

size_t deque_buf_size(size_t n,size_t sz)
{
    return n!=0 ? (n/sz) : (sz<512 ? (512/sz) : 1);
}

template <class T,size_t BufSiz>
struct deque_iterator
{

    //typedefs
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef ptrdiff_t                   difference_type;

    typedef deque_iterator<T,BufSiz>    iterator;
    typedef T**                         map_pointer;


    //buffer_size
    static size_t buffer_size(){return deque_buf_size(BufSiz,sizeof(T));}


    //wrapped pointers
    pointer _cur;
    pointer _node_begin;
    pointer _node_end;
    map_pointer _node;


    //jump node
    void set_node(map_pointer new_node)
    {
        _node=new_node;
        _node_begin=*_node;
        _node_end=_node_begin+buffer_size();
    }


    //data access
    reference operator*(){return *_cur;}

    pointer operator->(){return &(operator*());}

    reference operator[](difference_type n) const{return *(*this+n);}


    //arithmetic operators
    difference_type operator-(const iterator& iter) const
    {
        return buffer_size()*(_node-iter._node-1)+(_cur-_node_begin)+(iter._node_end-iter._cur);
    }

    iterator& operator++()
    {
        ++_cur;
        if(_cur==_node_end){
            set_node(_node+1);
            _cur=_node_begin;
        }
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
        if(_cur==_node_begin){
            set_node(_node-1);
            _cur=_node_end;
        }
        --_cur;
        return *this;
    }

    iterator operator--(int)
    {
        iterator tmp=*this;
        --*this;
        return tmp;
    }

    iterator& operator+=(difference_type n)
    {
        difference_type offset=_cur-_node_begin+n;
        if(offset>=0 && offset<static_cast<difference_type>(buffer_size()))
            _cur+=n;
        else{
            difference_type node_offset=offset>0? offset/buffer_size() : -((-offset-1)/buffer_size())-1;   //trap
            set_node(_node+node_offset);
            _cur=_node_begin+(offset-node_offset*buffer_size());
        }
        return *this;
    }

    iterator operator+(difference_type n) const
    {
        iterator tmp=*this;
        return tmp+=n;
    }

    iterator& operator-=(difference_type n)
    {
        return *this+=-n;
    }

    iterator operator-(difference_type n) const
    {
        iterator tmp=*this;
        return tmp-=n;
    }


    //relational operators
    bool operator==(const iterator& iter) const{return _cur==iter._cur;}

    bool operator!=(const iterator& iter) const{return !operator==(iter);}

};

//deque
template <class T,size_t BufSiz=0,class Alloc=malloc_alloc>
class deque
{
public:

    typedef T                           value_type;
    typedef deque_iterator<T,BufSiz>    iterator;
    typedef reverse_iterator<iterator>  reverse_iterator;
    typedef size_t                      size_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef const T&                    const_reference;
    typedef ptrdiff_t                   difference_type;


    //constructors
    deque(){initialize_n();}

    explicit deque(size_type n){initialize_n(n);}

    explicit deque(size_type n,value_type value):deque(){insert_fill(begin(),value,n);}

    deque(const deque& de):deque(){insert_range(begin(),de.begin(),de.end());}

    template <class InputIterator>
    deque(InputIterator begin,InputIterator end):deque()
    {
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        insert_dispatch(_begin,begin,end,isInteger());
    }

    deque(std::initializer_list<value_type> il):deque(){insert_range(begin(),il.begin(),il.end());}


    //iterators
    iterator begin() const{return _begin;}

    iterator end() const{return _end;}

    reverse_iterator rbegin() const {return reverse_iterator(end());}

    reverse_iterator rend() const {return reverse_iterator(begin());}


    //assignment
    deque& operator=(const deque& de)
    {
        free();initialize_n();
        insert_range(begin(),de.begin(),de.end());
        return *this;
    }

    deque& operator=(std::initializer_list<value_type> il)
    {
        free();initialize_n();
        insert_range(begin(),il.begin(),il.end());
        return *this;
    }


    //swap & assign
    void swap(deque& de)
    {
        map_pointer tmp_map=de._map;de._map=_map;_map=tmp_map;
        size_type tmp_map_size=de._map_size;de._map_size=_map_size;_map_size=tmp_map_size;
        iterator tmp_begin=de._begin;de._begin=_begin;_begin=tmp_begin;
        iterator tmp_end=de._end;de._end=_end;_end=tmp_end;
    }

    deque& assign(size_type n,value_type value)
    {
        free();initialize_n();
        insert_fill(begin(),value,n);
        return *this;
    }

    template <class InputIterator>
    deque& assign(InputIterator begin,InputIterator end)
    {
        free();initialize_n();
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        insert_dispatch(_begin,begin,end,isInteger());
        return *this;
    }

    deque& assign(std::initializer_list<value_type> il)
    {
        return operator=(il);
    }


    //size
    size_type size() const{return _end-_begin;}

    bool empty() const{return _begin==_end;}


    //data access
    reference front(){return *_begin;}

    reference back(){iterator tmp=_end;--tmp;return *tmp;}

    reference operator[](size_type n){return _begin[n];}


    //insert
    void push_back(const_reference value){insert_fill(end(),value);}

    template <typename... Args>
    void emplace_back(const Args... args){emplace(end(),args...);}

    void push_front(const_reference value){insert_fill(begin(),value);}

    template <typename... Args>
    void emplace_front(const Args... args){emplace(begin(),args...);}

    iterator insert(iterator tar,const_reference value){return insert_fill(tar,value);}

    template <typename... Args>
    iterator emplace(iterator tar,const Args... args){return emplace_aux(tar,args...);}

    iterator insert(iterator tar,size_type n,const_reference value){return insert_fill(tar,value,n);}

    template <class InputIterator>
    iterator insert(iterator tar,InputIterator begin,InputIterator end)
    {
        typedef typename type_traits<InputIterator>::isInteger isInteger;
        return insert_dispatch(tar,begin,end,isInteger());
    }

    iterator insert(iterator tar,std::initializer_list<T> il){return insert_range(tar,il.begin(),il.end());};


    //erase
    void pop_front(){erase_n(begin());}

    void pop_back(){erase_n(end()-1);}

    iterator erase(iterator tar){return erase_n(tar);}

    iterator erase(iterator begin,iterator end){return erase_n(begin,end-begin);}

    void clear(){destroy(_begin,_end);_end=_begin;}


    //resize
    void resize(size_type n);

    void resize(size_type n,value_type value);

    void shrink_to_fit();


    //destructor
    ~deque(){free();}

private:

    typedef pointer* map_pointer;
    typedef simple_alloc<value_type,Alloc> data_allocator;
    typedef simple_alloc<pointer,Alloc> map_allocator;

    map_pointer _map=nullptr;
    size_type _map_size=0;
    iterator _begin;
    iterator _end;


    //buffer_size
    static size_t buffer_size(){return deque_buf_size(BufSiz,sizeof(T));}


    //initialize
    void initialize_n(size_type num_elements=0)
    {
        size_type num_nodes=num_elements/buffer_size()+1;
        _map_size=num_nodes+2;
        _map=map_allocator::allocate(_map_size);

        map_pointer map_begin=_map+(_map_size-num_nodes)/2;
        map_pointer map_end=map_begin+num_nodes;

        for(map_pointer map_cur=map_begin;map_cur!=map_end;++map_cur)
            *map_cur=data_allocator::allocate(buffer_size());

        _begin.set_node(map_begin);
        _begin._cur=_begin._node_begin;
        _end.set_node(--map_end);
        _end._cur=_end._node_begin+num_elements%buffer_size();

        uninitialized_fill(_begin,_end);
    }


    //memory related
    void free()
    {
        map_pointer map_begin=_begin._node;
        map_pointer map_end=_end._node+1;

        clear();
        for(map_pointer map_cur=map_begin;map_cur!=map_end;++map_cur)
            data_allocator::deallocate(*map_cur);
        destroy(map_begin,map_end);
        map_allocator::deallocate(_map);
    }
    void chk_front_alloc(size_type n=1)
    {
        //calculate the number of nodes need to add.
        size_type offset=static_cast<size_type>(_begin._cur-_begin._node_begin);
        size_type extra_num_nodes=(offset<n ? (n-offset-1)/buffer_size()+1 : 0);
        if(!extra_num_nodes) return;

        //save the "_node" status
        difference_type map_offset=_begin._node-_map;
        difference_type begin_offset_end=_end._node-_begin._node;     //_begin._node&_end._node is []

        //expand map-front
        size_type extra_alloc=map_offset;
        while(extra_alloc<=extra_num_nodes)
            extra_alloc*=2;

        //realloc the _map
        _map_size=extra_alloc-map_offset+_map_size;
        map_pointer tmp_map=map_allocator::allocate(_map_size);
        uninitialized_copy(_begin._node,_end._node+1,tmp_map+extra_alloc);
        map_allocator::deallocate(_map);
        _map=tmp_map;

        //cause the map has be realloced,the node of iterator should be altered simultaneously.
        _begin.set_node(_map+extra_alloc);
        _end.set_node(_begin._node+begin_offset_end);

        //generate nodes
        map_pointer map_cur=_begin._node-1;
        while(extra_num_nodes--){
            *map_cur=data_allocator::allocate(buffer_size());
            --map_cur;
        }

    }

    void chk_back_alloc(size_type n=1)
    {
        //calculate the number of nodes need to add.
        size_type offset=static_cast<size_type>(_end._node_end-_end._cur)-1;
        size_type extra_num_nodes=(n>offset ? (n-offset-1)/buffer_size()+1 : 0);
        if(!extra_num_nodes) return;
        //save the "_node" status
        difference_type map_begin_offset=_begin._node-_map;
        difference_type begin_end_offset=_end._node-_begin._node;
        difference_type end_map_offset=_map+_map_size-_end._node-1;
        //expand map-back
        size_type extra_alloc=end_map_offset;
        while(extra_alloc<=extra_num_nodes)
            extra_alloc*=2;
        //realloc the _map
        _map_size=extra_alloc-end_map_offset+_map_size;
        map_pointer tmp_map=map_allocator::allocate(_map_size);
        uninitialized_copy(_begin._node,_end._node+1,tmp_map+map_begin_offset);
        map_allocator::deallocate(_map);
        _map=tmp_map;
        //cause the map has be realloced,the node of iterator should be altered simultaneously.
        _begin.set_node(_map+map_begin_offset);
        _end.set_node(_begin._node+begin_end_offset);
        //generate nodes
        map_pointer map_cur=_end._node+1;
        while(extra_num_nodes--){
            *map_cur=data_allocator::allocate(buffer_size());
            ++map_cur;
        }
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
    iterator emplace_aux(iterator tar,const Args... args)
    {
        size_type offset=static_cast<size_type>(tar-_begin);
        iterator ret;
        if(offset<size()/2){
            chk_front_alloc();
            ret=_begin--;
            for(;ret!=tar;++ret){       //compare ret._cur & tar._cur
                construct((ret-1)._cur,*ret);
                destroy(ret._cur);
            }
            construct((--ret)._cur,args...);
        }else{
            difference_type copy_size=_end-tar;
            chk_back_alloc();          //tar is not original tar any more( _node has be changed).

            iterator ret=_end++;
            while(copy_size--){
                --ret;
                construct((ret+1)._cur,*ret);
                destroy(ret._cur);
            }
            construct(ret._cur,args...);
        }
        return ret;
    }

    iterator insert_fill(iterator tar,value_type value,size_type n=1)
    {
        size_type offset=static_cast<size_type>(tar-_begin);
        iterator ret;
        if(offset<size()/2){
            chk_front_alloc(n);
            ret=_begin;
            for(;ret!=tar;++ret){       //compare ret._cur & tar._cur
                construct((ret-n)._cur,*ret);
                destroy(ret._cur);
            }
            _begin-=n;
            uninitialized_fill_n(ret-=n,n,value);
        }else{
            difference_type copy_size=_end-tar;
            chk_back_alloc(n);          //tar is not original tar any more( _node has be changed).

            iterator ret=_end;
            while(copy_size--){
                --ret;
                construct((ret+n)._cur,*ret);
                destroy(ret._cur);
            }
            _end+=n;
            uninitialized_fill_n(ret,n,value);
        }
        return ret;
    }

    template <class InputIterator>
    iterator insert_range(iterator tar,InputIterator begin,InputIterator end)
    {
        size_type offset=static_cast<size_type>(tar-_begin);
        size_type n=end-begin;      //
        iterator ret;
        if(offset<size()/2){
            chk_front_alloc(n);
            ret=_begin;
            for(;ret!=tar;++ret){       //compare ret._cur & tar._cur
                construct((ret-n)._cur,*ret);
                destroy(ret._cur);
            }
            _begin-=n;
            uninitialized_copy(begin,end,ret-=n);   //
        }else{
            difference_type copy_size=_end-tar;
            chk_back_alloc(n);          //tar is not original tar any more.

            iterator ret=_end;
            while(copy_size--){
                --ret;
                construct((ret+n)._cur,*ret);
                destroy(ret._cur);
            }
            _end+=n;
            uninitialized_copy(begin,end,ret);  //
        }
        return ret;

    }


    //erase aux
    iterator erase_n(iterator tar,size_type n=1)
    {
        size_type offset=static_cast<size_type>(tar-_begin);
        iterator ret=tar+n;
        destroy(tar,n);
        if(offset<size()/2){
            while(offset--){
                --tar;
                construct((tar+n)._cur,*tar);
                destroy(tar._cur);
            }
            _begin+=n;
        }else{
            for(iterator tmp=tar+n;tmp!=_end;++tmp){
                construct((tmp-n)._cur,*tmp);
                destroy(tmp._cur);
            }
            _end-=n;
        }
        return ret;
    }

};

SSTL_NAMESPACE_END

#endif // DEQUE_H_INCLUDED
