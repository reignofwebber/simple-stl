#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "sstl.h"
#include "allocator.h"
#include "iterator.h"
#include "function.h"
#include "utilities.h"
#include "algorithm.h"



SSTL_NAMESPACE_BEGIN

typedef  bool rb_tree_color_type;

constexpr rb_tree_color_type rb_tree_black=true;
constexpr rb_tree_color_type rb_tree_red=false;


//inorder
template <class PNode,class Function>
void dfs_search_inorder(PNode root,Function func)
{
    if(!root) return;
    func(root);
    if(root->left)
        dfs_search_inorder(root->left,func);
    if(root->right)
        dfs_search_inorder(root->right,func);

}

//preorder
template <class PNode,class Function>
void dfs_search_preorder(PNode root,Function func)
{
    if(!root) return;
    if(root->left)
        dfs_search_preorder(root->left,func);
    func(root);
    if(root->right)
        dfs_search_preorder(root->right,func);

}

template <class PNode>
PNode tree_rotate_left(PNode x)
{
    PNode y=x->right;
    x->right=y->left;
    if(x->right)
        x->right->parent=x;

    y->parent=x->parent;
    if(x->parent->parent==x)
        x->parent->parent=y;
    else if(x->parent->left==x)
        x->parent->left=y;
    else if(x->parent->right==x)
        x->parent->right=y;

    y->left=x;
    x->parent=y;
    return y;
}

template <class PNode>
PNode tree_rotate_right(PNode x)
{
    PNode y=x->left;
    x->left=y->right;
    if(x->left)
        x->left->parent=x;

    y->parent=x->parent;
    if(x->parent->parent==x)
        x->parent->parent=y;
    else if(x->parent->left==x)
        x->parent->left=y;
    else if(x->parent->right==x)
        x->parent->right=y;

    y->right=x;
    x->parent=y;
    return y;
}


template <class T>
struct rb_tree_node
{
    typedef rb_tree_color_type color_type;

    rb_tree_node* left;
    rb_tree_node* right;
    rb_tree_node* parent;
    color_type color;
    T value;
};


template <class T>
struct rb_tree_iterator
{
    //typedefs

    typedef bidirectional_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T&                          reference;
    typedef T*                          pointer;
    typedef ptrdiff_t                   difference_type;

    typedef rb_tree_iterator            iterator;
    typedef rb_tree_node<T>*            link_type;


    //wrapped pointer
    link_type _node;


    //constructors
    rb_tree_iterator()=default;

    rb_tree_iterator(link_type node):_node(node){}

    rb_tree_iterator(const iterator& iter):_node(iter._node){}


    //data access
    reference operator*() const {return _node->value;}

    pointer operator->() const {return &(operator*());}


    //arithmetic operations
    iterator& operator++()
    {
        if(_node->right){
            _node=_node->right;
            while(_node->left)
                _node=_node->left;
        }else{
            while(_node->parent->right==_node && _node->parent->parent!=_node)
                _node=_node->parent;
            _node=_node->parent;

        }
        return *this;
    }

    iterator& operator--()
    {
        if(_node->parent->parent==_node && _node->left && _node->left->parent!=_node)
            _node=_node->right;
        else if(_node->left){
            _node=_node->left;
            while(_node->right)
                _node=_node->right;
        }else{
            while(_node->parent->left==_node && _node->parent->parent!=_node)
                _node=_node->parent;
            _node=_node->parent;
        }
        return *this;
    }

    iterator operator++(int)
    {
        iterator tmp=*this;
        ++*this;
        return tmp;
    }

    iterator operator--(int)
    {
        iterator tmp=*this;
        --*this;
        return tmp;
    }


    //relational operations
    bool operator==(const iterator& iter) const {return _node==iter._node;}

    bool operator!=(const iterator& iter) const {return _node!=iter._node;}

};


template <class Key,class Value,class KeyOfValue,class Compare=less<Key>,class Alloc=malloc_alloc>
class rb_tree
{
public:

    typedef Key         key_type;
    typedef Value       value_type;
    typedef Compare     key_compare;

    typedef rb_tree_iterator<value_type>        iterator;
    typedef reverse_iterator<iterator>          reverse_iterator;
    typedef const_iterator<reverse_iterator>    const_reverse_iterator;
    typedef const_iterator<iterator>            const_iterator;
    typedef size_t                              size_type;
    typedef value_type*                         pointer;
    typedef const value_type*                   const_pointer;
    typedef value_type&                         reference;
    typedef const value_type&                   const_reference;
    typedef ptrdiff_t                           difference_type;

    typedef rb_tree_node<value_type>            rb_tree_node;
    typedef rb_tree_node*                       link_type;


    //constructors
    rb_tree(const key_compare& comp=key_compare()):_size(0),_comp(comp){empty_initialize();}

    rb_tree(const rb_tree& t):_comp(t._comp)
    {
        empty_initialize();
        insert_equal(t.cbegin(),t.cend());
    }



    //assignment
    rb_tree& operator=(const rb_tree& t)
    {
        _comp=t._comp;
        clear();
        insert_equal(t.cbegin(),t.cend());
        return *this;
    }


    //swap
    void swap(rb_tree& t)
    {
        link_type tmp_header=_header;_header=t._header;t._header=tmp_header;
        size_type tmp_size=_size;_size=t._size;t._size=tmp_size;
        key_compare tmp_comp=_comp;_comp=t._comp;t._comp=tmp_comp;
    }

    key_compare key_comp() const {return _comp;}



    //iterators
    iterator begin() const {return leftmost();}

    iterator end() const {return _header;}

    const_iterator cbegin() const {return const_iterator(begin());}

    const_iterator cend() const {return const_iterator(end());}

    reverse_iterator rbegin() const {return reverse_iterator(end());}

    reverse_iterator rend() const {return reverse_iterator(begin());}

    const_reverse_iterator crbegin() const {return const_reverse_iterator(rbegin());}

    const_reverse_iterator crend() const {return const_reverse_iterator(rend());}


    //size
    bool empty() const {return _size==0;}

    size_type size() const {return _size;}


    //insert
    pair<iterator,bool> insert_unique(const_reference value){
        iterator ret=find(KeyOfValue()(value));
        if(ret!=end()) return {ret,false};

        link_type p=create_node(value);
        insert_node(p,insert_find(value));
        insert_rebalance(p);
        ++_size;
        return {p,true};
    }

    iterator insert_equal(const_reference value)
    {
        link_type p=create_node(value);
        insert_node(p,insert_find(value));
        insert_rebalance(p);
        ++_size;
        return p;
    }

    template <class InputIterator>
    void insert_unique(InputIterator begin,InputIterator end)
    {
        for(;begin!=end;++begin)
            insert_unique(*begin);
    }

    template <class InputIterator>
    void insert_equal(InputIterator begin,InputIterator end)
    {
        for(;begin!=end;++begin)
            insert_equal(*begin);

    }


    //erase
    iterator erase(iterator tar)
    {
        link_type p=tar._node;

        if(tar._node->left && tar._node->right){
            p=tar._node->right;
            while(p->left)
                p=p->left;

            tar._node->value=p->value;
            --tar;                          //--tar is the really node deleted.
        }
        ++tar;

        erase_rebalance(p);

        erase_node(p);
        --_size;
        return tar;

    }

    void erase(const key_type& k)
    {
        pair<iterator,iterator> range=equal_range(k);
        erase(range.first,range.second);
    }

    iterator erase(iterator begin,iterator end)
    {
        size_type n=sstl::distance(begin,end);
        if(!n) return begin;
        iterator after=erase(begin);
        while(--n)
            after=erase(after);

        return after;
    }

    void clear()
    {
        dfs_destroy(_header->parent);
        _header->parent=nullptr;
        _header->left=_header;
        _header->right=_header;
        _size=0;
    }


    //unique(key)
    iterator find(const key_type& k) const
    {
        link_type prev=_header;
        link_type cur=_header->parent;

        while(cur){
            if(!_comp(get_key(cur),k)){
                prev=cur;
                cur=cur->left;
            }else{
                cur=cur->right;
            }
        }
        iterator iter(prev);
        return (iter==end() || _comp(k,get_key(prev))) ? end() : iter;
    }

    size_type count(const key_type& k) const
    {
        iterator iter=find(k);
        if(iter==end()) return 0;
        size_type cnt=1;
        reverse_iterator left=reverse_iterator(iter);
        while(left!=rend() && !_comp(KeyOfValue()(*left++),k))
            ++cnt;
        iterator right=++iter;
        while(right!=end() && !_comp(k,KeyOfValue()(*right++)))
            ++cnt;

        return cnt;
    }

    iterator lower_bound(const key_type& k) const
    {
        difference_type len=_size;
        difference_type half;
        iterator middle,left=begin();
        while(len>0){
            half=len >> 1;
            middle=sstl::advance(left,half);
            if(_comp(KeyOfValue()(*middle),k)){
                left=middle;
                ++left;
                len=len-half-1;
            }else{
                len=half;
            }
        }
        return left;
    }

    iterator upper_bound(const key_type& k) const
    {
        difference_type len=_size;
        difference_type half;
        iterator middle,left=begin();
        while(len>0){
            half=len >> 1;
            middle=sstl::advance(left,half);
            if(_comp(k,KeyOfValue()(*middle))){
                len=half;
            }else{
                left=middle;
                ++left;
                len=len-half-1;
            }
        }
        return left;
    }

    pair<iterator,iterator> equal_range(const key_type& k) const
    {
        return {lower_bound(k),upper_bound(k)};
    }


    //extra
    link_type root() const {return _header->parent;}


    //destructor
    ~rb_tree() {clear();put_node(_header);}

private:
    typedef simple_alloc<rb_tree_node,Alloc>    tree_node_allocator;

    link_type _header;
    size_type _size;
    key_compare _comp;


    //min and max
    link_type leftmost() const {return _header->left;}

    link_type rightmost() const {return _header->right;}


    //value->key
    const key_type& get_key(link_type p) const
    {
        return KeyOfValue()(p->value);
    }


    //allocate and deallocate
    link_type get_node(){return tree_node_allocator::allocate();}

    void put_node(link_type p){tree_node_allocator::deallocate(p);}


    //create and destroy
    link_type create_node(const_reference value)
    {
        link_type p=get_node();
        construct(&p->value,value);
        p->color=rb_tree_red;
        p->left=nullptr;
        p->right=nullptr;
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
        _header=get_node();
        _header->color=rb_tree_red;
        _header->parent=nullptr;
        _header->left=_header;
        _header->right=_header;
    }


    //postorder-destroy(for clear())
    void dfs_destroy(link_type root)
    {
        if(!root) return;
        if(root->left)
            dfs_destroy(root->left);
        if(root->right)
            dfs_destroy(root->right);
        destroy_node(root);
    }


    //black or red
    bool is_black(link_type p)
    {
        return p==nullptr || p->color==rb_tree_black;
    }

    bool is_red(link_type p)
    {
        return !is_black(p);
    }


    //insert aux
    pair<link_type,bool> insert_find(const_reference value)
    {
        bool direc=true;
        link_type prev=_header;
        link_type cur=_header->parent;

        while(cur){
            prev=cur;
            direc=_comp(KeyOfValue()(value),get_key(cur));
            cur= direc ? cur->left : cur->right;

        }
        return {prev,direc};
    }


    //insert and erase node
    void insert_node(link_type tar,pair<link_type,bool> pos)
    {
        link_type p=pos.first;
        bool direc=pos.second;

        if(p==_header){
            tar->parent=_header;
            _header->parent=tar;
            tar->color=rb_tree_black;
            _header->left=tar;
            _header->right=tar;

        }else if(direc){
            tar->parent=p;
            p->left=tar;
            if(p==_header->left)
                _header->left=tar;

        }else {
            tar->parent=p;
            p->right=tar;
            if(p==_header->right)
                _header->right=tar;
        }
    }

    void erase_node(link_type tar)
    {
        if(tar->left || tar->right){
            link_type child= (tar->left!=nullptr ? tar->left : tar->right);

            if(_header->left==tar)
                _header->left=child;
            if(_header->right==tar)
                _header->right=child;

            child->parent=tar->parent;
            if(child->parent->parent==tar){
                child->parent->parent=child;
                child->color=rb_tree_black;
            }else if(child->parent->left==tar)
                child->parent->left=child;
            else
                child->parent->right=child;
        }else{
            link_type x=tar->parent;

            if(_header->left==tar)
                _header->left=x;
            if(_header->right==tar)
                _header->right=x;

            if(x->parent==tar)
                x->parent=nullptr;
            else if(x->left==tar)
                x->left=nullptr;
            else
                x->right=nullptr;
        }
        destroy_node(tar);
    }

    //insert and erase rebalance
    void insert_rebalance(link_type tar)
    {
        if(tar->color!=rb_tree_red || tar->parent->color!=rb_tree_red) return;

        link_type tar_parent=tar->parent;
        link_type tar_grandparent=tar_parent->parent;
        link_type top=tar_grandparent;

        if(tar_grandparent->left==tar_parent){
            if(tar_grandparent->right && tar_grandparent->right->color==rb_tree_red){
                tar_parent->color=rb_tree_black;
                tar_grandparent->right->color=rb_tree_black;
                tar_grandparent->color=rb_tree_red;
            }else{
                if(tar_parent->right==tar){
                    tar_parent=tree_rotate_left(tar_parent);
                    tar=tar_parent->left;
                }
                tar_parent->color=rb_tree_black;
                tar_grandparent->color=rb_tree_red;
                top=tree_rotate_right(tar_grandparent);
            }

        }else{
            if(tar_grandparent->left && tar_grandparent->left->color==rb_tree_red){
                tar_parent->color=rb_tree_black;
                tar_grandparent->left->color=rb_tree_black;
                tar_grandparent->color=rb_tree_red;
            }else{
                if(tar->parent->left==tar){
                    tar_parent=tree_rotate_right(tar_parent);
                    tar=tar_parent->right;
                }
                tar_parent->color=rb_tree_black;
                tar_grandparent->color=rb_tree_red;
                top=tree_rotate_left(tar_grandparent);
            }
        }
        //guarantee root's color is black
        _header->parent->color=rb_tree_black;
        //
        insert_rebalance(top);
    }

    void erase_rebalance(link_type tar)
    {
        if(tar==_header->parent || is_red(tar)) return;                 //case 1 end

                                                                      //black node
        if(tar->left || tar->right){                                    //black node with a child node
            link_type child= (tar->left!=nullptr ? tar->left : tar->right);
            child->color=rb_tree_black;                             //case 2 end
        }else{                                                      //black leaf node
            link_type p=tar->parent;
            link_type b;

            if(tar==p->left){
                b=p->right;
                                            //tar is left node.
                if(is_red(b)){
                    p->color=rb_tree_red;
                    b->color=rb_tree_black;
                    tree_rotate_left(p);
                    b=p->right;
                }                                   //--->  b->color==black.

                link_type bl=b->left;
                link_type br=b->right;

                if(is_red(bl) && is_black(br)){
                    b->color=rb_tree_red;
                    bl->color=rb_tree_black;
                    tree_rotate_right(b);
                    br=b;
                    b=bl;
                }                                   //--->  b->color=black && br->color==red(if br exists).

                if(is_red(br)){
                    rb_tree_color_type tmp=p->color;        //swap p->color and b->color.
                    p->color=b->color;
                    b->color=tmp;
                    br->color=rb_tree_black;
                    tree_rotate_left(p);                    //case 3 end.
                }else{                                      //b is a leaf node.
                    if(is_red(p)){                          //p->color is red.
                        p->color=rb_tree_black;
                        b->color=rb_tree_red;               //case 4 end.
                    }else{
                        b->color=rb_tree_red;
                        erase_rebalance(p);                 //case 5 loop.
                    }
                }

            }else{
                b=p->left;

                if(is_red(b)){
                    p->color=rb_tree_red;
                    b->color=rb_tree_black;
                    tree_rotate_right(p);
                    b=p->left;
                }

                link_type bl=b->left;
                link_type br=b->right;

                if(is_red(br) && is_black(bl)){
                    b->color=rb_tree_red;
                    br->color=rb_tree_black;
                    tree_rotate_left(b);
                    bl=b;
                    b=br;
                }

                if(is_red(bl)){
                    rb_tree_color_type tmp=p->color;        //swap p->color and b->color.
                    p->color=b->color;
                    b->color=tmp;
                    bl->color=rb_tree_black;
                    tree_rotate_right(p);
                }else{
                    if(is_red(p)){                          //p->color is red.
                        p->color=rb_tree_black;
                        b->color=rb_tree_red;               //case 4 end.
                    }else{
                        b->color=rb_tree_red;
                        erase_rebalance(p);                 //case 5 loop.
                    }
                }

            }
        }

    }

};


SSTL_NAMESPACE_END


#endif // TREE_H_INCLUDED
