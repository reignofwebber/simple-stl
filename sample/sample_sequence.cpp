#include "util.h"




#define CONTAINER_INT DEQUE_INT

#define IS_VECTOR 0
#define IS_LIST 0
#define IS_DEQUE 1


using namespace sstl;


int main()
{
    //constructors
    println("constructors:");
    CONTAINER_INT con1;
    CONTAINER_INT con2(5);
    CONTAINER_INT con3(5,-1);
    CONTAINER_INT con4(con3);
    VECTOR_INT v{1,2,3,4};
    CONTAINER_INT con5(v.cbegin(),v.cend());
    CONTAINER_INT con6{6,7,8};
    PRINT(con1);PRINT(con2);PRINT(con3);PRINT(con4);PRINT(con5);PRINT(con6);
    PRINTR(con1);PRINTR(con2);PRINTR(con3);PRINTR(con4);PRINTR(con5);PRINTR(con6);

    //assignment
    println("\nassignment:");
    println("con1=con3 con2={1,2,3,4}");
    con1=con3;con2={1,2,3,4};
    PRINT(con1);PRINT(con2);

    //swap & assign
    println("\nswap & assign :");
    println("swap con1 con2");
    con1.swap(con2);
    PRINT(con1);PRINT(con2);
    println("assign(range)");
    con1.assign(con2.begin(),con2.end());
    PRINT(con1);
    println("assign(fill)");
    con1.assign(5,-2);
    PRINT(con1);
    println("assign({4,5,6,7})");
    con1.assign({4,5,6,7});
    PRINT(con1);

    //size
    println("\nsize :");
    println("con1.size",con1.size());
    assert(!con1.empty());

    //data access
    println("\ndata access :");
    println("con1.front",con1.front());
    println("con1.back",con1.back());
#if IS_VECTOR || IS_DEQUE
    println("con1[2]",con1[2]);
#endif
    //insert
    println("\ninsert :");
    println("push_back");
    con1.push_back(8);
    PRINT(con1);
#if IS_LIST || IS_DEQUE
    println("push_front");
    con1.push_front(3);
    PRINT(con1);
#endif
    println("insert");
    con1.insert(advance(con1.begin(),5),7);
    PRINT(con1);
    println("insert(fill)");
    con1.insert(advance(con1.begin(),3),5,5);
    PRINT(con1);
    println("insert(range)");
    con1.insert(con1.begin(),v.cbegin(),v.cend());
    PRINT(con1);
    println("insert({-3,-2,-1,0})");
    con1.insert(con1.begin(),{-3,-2,-1,0});
    PRINT(con1);

    //erase
    println("\nerase :");
#if IS_LIST || IS_DEQUE
    println("pop_front");
    con1.pop_front();
    PRINT(con1);
#endif
    println("pop_back");
    con1.pop_back();
    PRINT(con1);
    println("erase");
    con1.erase(advance(con1.begin(),3));
    PRINT(con1);
    println("erase(range)");
    con1.erase(advance(con1.begin(),4),advance(con1.begin(),8));
    PRINT(con1);

#if IS_VECTOR
    //resize
    println("\nresize :");
    println("resize(15)");
    con1.resize(15,-1);
    PRINT(con1);
    println("resize(5)");
    con1.resize(5,-1);
    PRINT(con1);
    println("capacity :",con1.capacity());
    con1.shrink_to_fit();
    println("after shrink :",con1.capacity());
    con1.reserve(10);
    println("after reserve :",con1.capacity());
#endif


    println("\nclear");
    con1.clear();
    assert(con1.empty());

    return 0;
}
/*
deque<int,malloc_alloc,16> de1;
    deque<int,malloc_alloc,16> de2(8);
    deque<int,malloc_alloc,16> de3(8,-1);
    PRINT(de1);PRINT(de2);PRINT(de3);
    println(de2.size());
*/
