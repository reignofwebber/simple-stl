#ifndef SET_H_INCLUDED
#define SET_H_INCLUDED

#include "tree.h"

#include <initializer_list>


SSTL_NAMESPACE_BEGIN

template <class Key,class Compare=less<Key>,class Alloc=malloc_alloc>
class set
{
public:

    //typedefs
    typedef Key     key_type;
    typedef Key     value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;

    typedef rb_tree<key_type,value_type,identity<value_type>,key_compare,Alloc> rep_type;

    //common
    typedef typename rep_type::const_iterator           iterator;
    typedef typename rep_type::const_iterator           const_iterator;
    typedef typename rep_type::const_reverse_iterator   reverse_iterator;
    typedef typename rep_type::const_reverse_iterator   const_reverse_iterator;

    typedef typename rep_type::const_pointer            pointer;
    typedef typename rep_type::const_pointer            const_pointer;
    typedef typename rep_type::const_reference          reference;
    typedef typename rep_type::const_reference          const_reference;
    typedef typename rep_type::size_type                size_type;
    typedef typename rep_type::difference_type          difference_type;


    //constructors
    set():t(key_compare()){}

    explicit set(const key_compare& comp):t(comp){}

    template <class InputIterator>
    set(InputIterator begin,InputIterator end):t(key_compare())
    {
        t.insert_unique(begin,end);
    }

    template <class InputIterator>
    set(InputIterator begin,InputIterator end,const key_compare& comp):t(comp)
    {
        t.insert_unique(begin,end);
    }

    set(std::initializer_list<key_type> il):t(key_compare())
    {
        t.insert_unique(il.begin(),il.end());
    }

    set(const set& s):t(s.t){}


    //assignment
    set& operator=(const set& s) {t=s.t; return *this;}


    //iterators
    iterator begin() const {return t.cbegin();}

    iterator end() const {return t.cend();}

    reverse_iterator rbegin() const {return t.crbegin();}

    reverse_iterator rend() const {return t.crend();}


    //swap
    void swap(set& s) {t.swap(s.t);}


    //size
    bool empty() const {return t.empty();}

    size_type size() const {return t.size();}


    //insert
    pair<iterator,bool> insert(const_reference value)
    {

        pair<typename rep_type::iterator,bool> ret=t.insert_unique(value);
        return {iterator(ret.first),ret.second};
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


    //erase
    void erase(iterator tar) {t.erase(tar.base());}

    void erase(const key_type& k) {t.erase(k);}

    void erase(iterator begin,iterator end) {t.erase(begin.base(),end.base());}

    void clear() {t.clear();}


    //unique
    iterator find(const key_type& k) const {return iterator(t.find(k));}

    size_type count(const key_type& k) const {return t.count(k);}

    iterator lower_bound(const key_type& k) const {return iterator(t.lower_bound(k));}

    iterator upper_bound(const key_type& k) const {return iterator(t.upper_bound(k));}

    pair<iterator,iterator> equal_range(const key_type& k) const
    {
        pair<typename rep_type::iterator,typename rep_type::iterator> ret=t.equal_range(k);
        return {iterator(ret.first),iterator(ret.second)};
    }


    //extra
    key_compare key_comp() const {return t.key_comp();}

    value_compare value_comp() const {return t.key_comp();}

    ~set(){}


private:

    rep_type t;

};



template <class Key,class Compare=less<Key>,class Alloc=malloc_alloc>
class multiset
{
public:

    //typedefs
    typedef Key     key_type;
    typedef Key     value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;

    typedef rb_tree<key_type,value_type,identity<value_type>,key_compare,Alloc> rep_type;

    //common
    typedef typename rep_type::const_iterator           iterator;
    typedef typename rep_type::const_iterator           const_iterator;
    typedef typename rep_type::const_reverse_iterator   reverse_iterator;
    typedef typename rep_type::const_reverse_iterator   const_reverse_iterator;

    typedef typename rep_type::const_pointer            pointer;
    typedef typename rep_type::const_pointer            const_pointer;
    typedef typename rep_type::const_reference          reference;
    typedef typename rep_type::const_reference          const_reference;
    typedef typename rep_type::size_type                size_type;
    typedef typename rep_type::difference_type          difference_type;


    //constructors
    multiset():t(key_compare()){}

    explicit multiset(const key_compare& comp):t(comp){}

    template <class InputIterator>
    multiset(InputIterator begin,InputIterator end):t(key_compare())
    {
        t.insert_equal(begin,end);
    }

    template <class InputIterator>
    multiset(InputIterator begin,InputIterator end,const key_compare& comp):t(comp)
    {
        t.insert_equal(begin,end);
    }

    multiset(std::initializer_list<key_type> il):t(key_compare())
    {
        t.insert_equal(il.begin(),il.end());
    }

    multiset(const multiset& ms):t(ms.t){}


    //assignment
    multiset& operator=(const multiset& ms) {t=ms.t; return *this;}


    //iterators
    iterator begin() const {return t.cbegin();}

    iterator end() const {return t.cend();}

    reverse_iterator rbegin() const {return t.crbegin();}

    reverse_iterator rend() const {return t.crend();}


    //swap
    void swap(multiset& ms) {t.swap(ms.t);}


    //size
    bool empty() const {return t.empty();}

    size_type size() const {return t.size();}


    //insert
    iterator insert(const_reference value)
    {
        return iterator(t.insert_equal(value));
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


    //erase
    void erase(iterator tar) {t.erase(tar.base());}

    void erase(const key_type& k) {t.erase(k);}

    void erase(iterator begin,iterator end) {t.erase(begin.base(),end.base());}

    void clear() {t.clear();}


    //unique
    iterator find(const key_type& k) const {return iterator(t.find(k));}

    size_type count(const key_type& k) const {return t.count(k);}

    iterator lower_bound(const key_type& k) const {return iterator(t.lower_bound(k));}

    iterator upper_bound(const key_type& k) const {return iterator(t.upper_bound(k));}

    pair<iterator,iterator> equal_range(const key_type& k) const
    {
        pair<typename rep_type::iterator,typename rep_type::iterator> ret=t.equal_range(k);
        return {iterator(ret.first),iterator(ret.second)};
    }


    //extra
    key_compare key_comp() const {return t.key_comp();}

    value_compare value_comp() const {return t.key_comp();}

    ~multiset(){}

private:

    rep_type t;

};



SSTL_NAMESPACE_END


#endif // SET_H_INCLUDED
