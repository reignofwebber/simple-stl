#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "tree.h"

SSTL_NAMESPACE_BEGIN

template <class Key,class Value,class Compare=less<Key>,class Alloc=malloc_alloc>
class map
{
public:

    //typedefs
    typedef Key                         key_type;
    typedef Value                       data_type;
    typedef Value                       mapped_type;
    typedef pair<key_type,data_type>    value_type;
    typedef Compare                     key_compare;

    class value_compare : public binary_function<value_type,value_type,bool>
    {
        friend class map;
    public:

        bool operator()(value_type x,value_type y) const {return _comp(x.first,y.first);}

    private:

        Compare _comp;

        value_compare(Compare comp):_comp(comp){}

    };

    typedef rb_tree<key_type,value_type,select1st<value_type>,key_compare,Alloc> rep_type;

    typedef typename rep_type::iterator                 iterator;
    typedef typename rep_type::const_iterator           const_iterator;
    typedef typename rep_type::reverse_iterator         reverse_iterator;
    typedef typename rep_type::const_reverse_iterator   const_reverse_iterator;
    typedef typename rep_type::pointer                  pointer;
    typedef typename rep_type::const_pointer            const_pointer;
    typedef typename rep_type::reference                reference;
    typedef typename rep_type::const_reference          const_reference;
    typedef typename rep_type::size_type                size_type;
    typedef typename rep_type::difference_type          difference_type;


    //constructors
    map():t(key_compare()){}

    explicit map(const key_compare& comp):t(comp){}

    template <class InputIterator>
    map(InputIterator begin,InputIterator end,const key_compare& comp=key_compare()):t(comp)
    {
        t.insert_unique(begin,end);
    }

    map(std::initializer_list<value_type> il):t(key_compare())
    {
        t.insert_unique(il.begin(),il.end());
    }

    map(const map& m):t(m.t){}


    //assignment
    map& operator=(const map& m) {t=m.t;return *this;}


    //iterators
    iterator begin() const {return t.begin();}

    iterator end() const {return t.end();}

    const_iterator cbegin() const {return t.cbegin();}

    const_iterator cend() const {return t.cend();}

    reverse_iterator rbegin() const {return t.rbegin();}

    reverse_iterator rend() const {return t.rend();}

    const_reverse_iterator crbegin() const {return t.crbegin();}

    const_reverse_iterator crend() const {return t.crend();}


    //swap
    void swap(map& m) {t.swap(m.t);}


    //size
    bool empty() const {return t.empty();}

    size_type size() const {return t.size();}


    //data access
    data_type& operator[](const key_type& k)
    {
        return (*(insert(value_type(k,data_type())).first)).second;
    }


    //insert
    pair<iterator,bool> insert(const_reference value)
    {
        return t.insert_unique(value);
    }

    template <class InputIterator>
    void insert(InputIterator begin,InputIterator end)
    {
        t.insert_unique(begin,end);
    }

    void insert(std::initializer_list<value_type> il)
    {
        t.insert_unique(il.begin(),il.end());
    }

    void erase(iterator tar) {t.erase(tar);}

    void erase(const key_type& k) {t.erase(k);}

    void erase(iterator begin,iterator end) {t.erase(begin,end);}

    void clear() {t.clear();}


    //unique
    iterator find(const key_type& k) const {return t.find(k);}

    size_type count(const key_type& k) const {return t.count(k);}

    iterator lower_bound(const key_type& k) const {return t.lower_bound(k);}

    iterator upper_bound(const key_type& k) const {return t.upper_bound(k);}

    pair<iterator,iterator> equal_range(const key_type& k) const {return t.equal_range(k);}


    //destructor
    ~map(){}

private:

    rep_type t;
};



template <class Key,class Value,class Compare=less<Key>,class Alloc=malloc_alloc>
class multimap
{
public:

    //typedefs
    typedef Key                         key_type;
    typedef Value                       data_type;
    typedef Value                       mapped_type;
    typedef pair<key_type,data_type>    value_type;
    typedef Compare                     key_compare;

    class value_compare : public binary_function<value_type,value_type,bool>
    {
        friend class multimap;
    public:

        bool operator()(value_type x,value_type y) const {return _comp(x.first,y.first);}

    private:

        Compare _comp;

        value_compare(Compare comp):_comp(comp){}

    };

    typedef rb_tree<key_type,value_type,select1st<value_type>,key_compare,Alloc> rep_type;

    typedef typename rep_type::iterator                 iterator;
    typedef typename rep_type::const_iterator           const_iterator;
    typedef typename rep_type::reverse_iterator         reverse_iterator;
    typedef typename rep_type::const_reverse_iterator   const_reverse_iterator;
    typedef typename rep_type::pointer                  pointer;
    typedef typename rep_type::const_pointer            const_pointer;
    typedef typename rep_type::reference                reference;
    typedef typename rep_type::const_reference          const_reference;
    typedef typename rep_type::size_type                size_type;
    typedef typename rep_type::difference_type          difference_type;


    //constructors
    multimap():t(key_compare()){}

    explicit multimap(const key_compare& comp):t(comp){}

    template <class InputIterator>
    multimap(InputIterator begin,InputIterator end,const key_compare& comp=key_compare()):t(comp)
    {
        t.insert_equal(begin,end);
    }

    multimap(std::initializer_list<value_type> il):t(key_compare())
    {
        t.insert_equal(il.begin(),il.end());
    }

    multimap(const multimap& mm):t(mm.t){}


    //assignment
    multimap& operator=(const multimap& mm) {t=mm.t;return *this;}


    //iterators
    iterator begin() const {return t.begin();}

    iterator end() const {return t.end();}

    const_iterator cbegin() const {return t.cbegin();}

    const_iterator cend() const {return t.cend();}

    reverse_iterator rbegin() const {return t.rbegin();}

    reverse_iterator rend() const {return t.rend();}

    const_reverse_iterator crbegin() const {return t.crbegin();}

    const_reverse_iterator crend() const {return t.crend();}


    //swap
    void swap(multimap& mm) {t.swap(mm.t);}


    //size
    bool empty() const {return t.empty();}

    size_type size() const {return t.size();}


    //insert
    iterator insert(const_reference value)
    {
        return t.insert_equal(value);
    }

    template <class InputIterator>
    void insert(InputIterator begin,InputIterator end)
    {
        t.insert_equal(begin,end);
    }

    void insert(std::initializer_list<value_type> il)
    {
        t.insert_equal(il.begin(),il.end());
    }

    void erase(iterator tar) {t.erase(tar);}

    void erase(const key_type& k) {t.erase(k);}

    void erase(iterator begin,iterator end) {t.erase(begin,end);}

    void clear() {t.clear();}


    //unique
    iterator find(const key_type& k) const {return t.find(k);}

    size_type count(const key_type& k) const {return t.count(k);}

    iterator lower_bound(const key_type& k) const {return t.lower_bound(k);}

    iterator upper_bound(const key_type& k) const {return t.upper_bound(k);}

    pair<iterator,iterator> equal_range(const key_type& k) const {return t.equal_range(k);}


    //destructor
    ~multimap(){}

private:

    rep_type t;

};




SSTL_NAMESPACE_END



#endif // MAP_H_INCLUDED
