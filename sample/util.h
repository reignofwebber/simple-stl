#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <iostream>
#include <string>


#include "../include/vector.h"
#include "../include/list.h"
#include "../include/deque.h"

#include "../include/set.h"
#include "../include/map.h"


#include "../include/algorithm.h"


#define TO_STRING(x) #x

#define NEXT_LINE cout << endl;
#define PRINT(x) print_container(TO_STRING(x),x)
#define PRINTR(x) printr_container(TO_STRING(x),x)

#define DEQUE_BUFSIZ 16

#define VECTOR_INT vector<int>
#define LIST_INT list<int>
#define DEQUE_INT deque<int,DEQUE_BUFSIZ>


using std::cout;
using std::cin;
using std::endl;
using std::string;

template <class Container>
void print_container(const string& name,Container& c)
{
    cout << name+" : "; //cout << name << " : ";
    for(auto &i : c)
        cout << i << " ";
    NEXT_LINE;
}

template<class Container>
void printr_container(const string& name,Container& c)
{
    cout << 'R'+name+" : ";
    for(typename Container::reverse_iterator riter=c.rbegin();riter!=c.rend();++riter)
        cout << *riter << " ";
    NEXT_LINE;
}

void println(){NEXT_LINE;}

template <typename T,typename... Args>
void println(const T& t,const Args&... args)
{
    cout << t << " ";
    println(args...);
}

void printc(){}

template <typename T,typename... Args>
void printc(const T& t,const Args&... args)
{
    cout << t ;
    printc(args...);
}

template <class T>
struct print_single{
    void operator()(const T& t){printc(t);}
};


class simple_book
{

};

#endif // UTILITY_H_INCLUDED
