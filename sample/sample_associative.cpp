#include "util.h"

#include "../include/tree.h"
#include "../include/set.h"
#include "../include/map.h"

#define IS_SET 0
#define IS_MAP 1

#define MULTIPLE 1

#if MULTIPLE
#define SET multiset
#define MAP multimap
#else
#define SET set
#define MAP map
#endif



using namespace sstl;

template <class PNode>
struct print_rb_tree_node
{
    void operator()(PNode x)
    {
        printc(x->value,",",x->color ? "black":"red"," | ");

    }
};


int main()
{
#if IS_SET
    //constructors and assignment
    SET<int> s1;
    vector<int> v{2,3,3,5,0};
    SET<int> s2(v.cbegin(),v.cend());
    SET<int> s3(s2);
    SET<int> s4{1,-2,3,3,7,5};
    SET<int,greater<int>> s5{1,-2,3,3,7,5};
    s1=s2;

    PRINT(s1);PRINT(s2);PRINT(s3);PRINT(s4);PRINT(s5);
    println("\nreverse :");
    PRINTR(s1);PRINTR(s2);PRINTR(s3);PRINTR(s4);PRINTR(s5);

    //swap
    println("\nswap :");
    s1.swap(s4);
    PRINT(s1);PRINT(s4);

    //size
    assert(!s1.empty());
#if MULTIPLE
    assert(s1.size()==6);
#else
    assert(s1.size()==5);
#endif

    //insert
    println("\ninsert :");
    println("insert single");
    s1.insert(8);PRINT(s1);
    println("insert range");
    s1.insert(v.cbegin(),v.cend());PRINT(s1);
    println("insert {1,2,3,4}");
    s1.insert({1,2,3,4});PRINT(s1);

    //erase
    println("\nerase :");
    println("erase single");
    s1.erase(s1.begin());PRINT(s1);
    println("erase key");
    s1.erase(3);PRINT(s1);
    println("erase range");
    s1.erase(s1.begin(),advance(s1.begin(),3));PRINT(s1);

    //unique
    println("\nunique :");
    assert(s1.find(1)==s1.end() && s1.find(8)!=s1.end());
    assert(!s1.count(1) && s1.count(8)==1);
#if MULTIPLE
    assert(s1.lower_bound(4)==advance(s1.begin(),2));
    assert(s1.upper_bound(4)==advance(s1.begin(),3));
    assert(s1.equal_range(5).first==advance(s1.begin(),3));
    assert(s1.equal_range(5).second==advance(s1.begin(),5));
    assert(s1.equal_range(6).first==s1.equal_range(6).second);
#else
    assert(s1.lower_bound(4)==advance(s1.begin(),1));
    assert(s1.upper_bound(4)==advance(s1.begin(),2));
    assert(s1.equal_range(5).first==advance(s1.begin(),2));
    assert(s1.equal_range(5).second==advance(s1.begin(),3));
    assert(s1.equal_range(6).first==s1.equal_range(6).second);
#endif
    //clear
    s1.clear();
    assert(s1.empty());

#else

    MAP<string,int> m1;
    MAP<string,int> m2{{"a",1},{"b",2},{"b",3},{"ab",3}};
    MAP<string,int> m3(m2);
    vector<pair<string,int>> v{{"d",4},{"e",5},{"f",6},{"g",7}};
    MAP<string,int> m4(v.cbegin(),v.cend());
    MAP<string,int,greater<string>> m5{{"a",1},{"b",2},{"b",3},{"ab",3}};
    m1=m2;

    PRINT(m1);PRINT(m2);PRINT(m3);PRINT(m4);PRINT(m5);
    println("\nreverse");
    PRINTR(m1);PRINTR(m2);PRINTR(m3);PRINTR(m4);PRINTR(m5);

    //size
    assert(!m1.empty());
#if MULTIPLE
    assert(m1.size()==4);
#else
    assert(m1.size()==3);
#endif

    //insert
    println("\ninsert :");
    println("insert single");
    m1.insert({"c",4});PRINT(m1);
    println("insert range");
    m1.insert(v.cbegin(),v.cend());PRINT(m1);
    println("insert {{'g',8},{'h',9}}");
    m1.insert({{"g",8},{"h",9}});PRINT(m1);

    //erase
    println("\nerase :");
    println("erase single");
    m1.erase(m1.begin());PRINT(m1);
    println("erase key");
    m1.erase("g");PRINT(m1);
    println("erase range");
    m1.erase(m1.begin(),advance(m1.begin(),3));PRINT(m1);

#if !MULTIPLE
    //data access
    println("\ndata access :");
    m1["g"]=7;
    m1["h"]=8;
    m1["c"]=3;
    PRINT(m1);
#endif
    //unique
    println("\nunique :");
    assert(m1.find("a")==m1.end() && m1.find("h")!=m1.end());
    assert(!m1.count("a") && m1.count("h")==1);
    assert(m1.lower_bound("d")==advance(m1.begin(),1));
    assert(m1.upper_bound("d")==advance(m1.begin(),2));
    assert(m1.equal_range("e").first==advance(m1.begin(),2));
    assert(m1.equal_range("e").second==advance(m1.begin(),3));
    assert(m1.equal_range("a").first==m1.equal_range("a").second);


    //clear
    m1.clear();
    assert(m1.empty());

#endif

    return 0;
}

/**
typedef typename rb_tree<float,float,identity<float>>::iterator iterator;
    rb_tree<float,float,identity<float>> tree;
    tree.insert_unique(10);
    tree.insert_unique(7);
    tree.insert_unique(6);
    tree.insert_equal(6);
    tree.insert_unique(11);
    tree.insert_unique(12);
    tree.insert_unique(8);
    tree.insert_unique(9);
    tree.insert_unique(8.5);
    tree.insert_unique(12);
    tree.insert_equal(12);
    rb_tree<float,float,identity<float>> tree1(tree);

    tree.erase(6);

    rb_tree<float,float,identity<float>>::const_iterator iter=tree.cbegin();
    for(;iter!=tree.cend();++iter)
        println(*iter);



    println("...................");
    rb_tree<float,float,identity<float>>::const_reverse_iterator riter=tree.crbegin();

    for(;riter!=tree.crend();++riter)
        println(*riter);



    dfs_search_inorder(tree.root(),print_rb_tree_node<rb_tree_node<float>*>());
    println();
   // dfs_search_preorder(tree.root(),print_rb_tree_node<rb_tree_node<float>*>());
   // println();

    assert(tree.find(2)==tree.end());

    tree.erase(advance(tree.begin(),4));
    tree.erase(advance(tree.begin(),5));
    tree.erase(advance(tree.begin(),5));
    tree.erase(advance(tree.begin(),4));
    tree.erase(advance(tree.begin(),3));
    tree.erase(advance(tree.begin(),2));
    tree.erase(advance(tree.begin(),1));


    //for(iter=tree.begin();iter!=tree.end();++iter)
    //    println(*iter);

    dfs_search_inorder(tree.root(),print_rb_tree_node<rb_tree_node<float>*>());
    println();

*/
