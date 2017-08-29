#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include "sstl.h"


SSTL_NAMESPACE_BEGIN

template <class Args,class Result>
struct unary_function
{
    typedef Args argument_type;
    typedef Result result_type;
};


template <class Arg1,class Arg2,class Result>
struct binary_function
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};


//arithmetic functions
template <class T>
struct plus : public binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const {return x+y;}
};


template <class T>
struct minus : public binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const {return x-y;}
};


template <class T>
struct multiplies : public binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const {return x*y;}
};


template <class T>
struct divides : public binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const {return x/y;}
};


template <class T>
struct modulus : public binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const {return x%y;}
};


template <class T>
struct negate : public unary_function<T,T>
{
    T operator()(const T& x) const {return -x;}
};


//identity_element
template <class T>
T identity_element(plus<T>){return T(0);}

template <class T>
T identity_element(multiplies<T>){return T(1);}


//relational functions
template <class T>
struct equal_to : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const {return x==y;}
};


template <class T>
struct not_equal_to : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const {return x!=y;}
};


template <class T>
struct greater : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const {return x>y;}
};


template <class T>
struct greater_equal : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const {return x>=y;}
};


template <class T>
struct less : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const {return x<y;}
};


template <class T>
struct less_equal : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const {return x<=y;}
};


//logical functions
template <class T>
struct logical_and : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const {return x&&y;}
};


template <class T>
struct logical_or : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const {return x||y;}
};


template <class T>
struct logical_not : public unary_function<T,bool>
{
    bool operator()(const T& x) const {return !x;}
};


//identity select and project
template <class T>
struct identity : public unary_function<T,T>
{
    const T& operator()(const T& x) const {return x;}
};


template <class Pair>
struct select1st : public unary_function<Pair,typename Pair::first_type>
{
    const typename Pair::first_type& operator()(const Pair& x) const {return x.first;}
};


template <class Pair>
struct select2nd : public unary_function<Pair,typename Pair::second_type>
{
    const typename Pair::second_type& operator()(const Pair& x) const {return x.second;}
};


template <class Arg1,class Arg2>
struct project1st : public binary_function<Arg1,Arg2,Arg1>
{
    Arg1 operator()(const Arg1& x,const Arg2&) const {return x;}
};


template <class Arg1,class Arg2>
struct project2nd : public binary_function<Arg1,Arg2,Arg2>
{
    Arg2 operator()(const Arg1&,const Arg2& x) const {return x;}
};




template <class Predicate>
class unary_negate : public unary_function<typename Predicate::argument_type,bool>
{
public:
    explicit unary_negate(const Predicate& x):pred(x){}

    bool operator()(const typename Predicate::argument_type x) const
    {
        return !pred(x);
    }

private:

    Predicate pred;

};

template <class Predicate>
unary_negate<Predicate> not1(const Predicate& pred)
{
    return unary_negate<Predicate>(pred);
}


template <class Predicate>
class binary_negate : public binary_function<typename Predicate::first_argument_type,
                                                        typename Predicate::second_argument_type,
                                                        bool>
{
public:

    explicit binary_negate(const Predicate& x):pred(x){}

    bool operator()(const typename Predicate::first_argument_type& x,
                    const typename Predicate::second_argument_type& y) const
    {
        return !pred(x,y);
    }

private:

    Predicate pred;
};

template <class Predicate>
binary_negate<Predicate> not2(const Predicate& pred)
{
    return binary_negate<Predicate>(pred);
}


template <class Operation>
class binder1st : public unary_function<typename Operation::second_argument_type,
                                        typename Operation::result_type>
{
public:

    binder1st(const Operation& x,const typename Operation::first_argument_type& y):op(x),value(y){}

    typename Operation::result_type
    operator()(const typename Operation::second_argument_type& x) const
    {
        return op(value,x);
    }

private:

    Operation op;
    typename Operation::first_argument_type value;
};


template <class Operation,class T>
binder1st<Operation> bind1st(const Operation& op,const T& x)
{
    typename Operation::first_argument_type value=x;
    return binder1st<Operation>(op,value);
}


template <class Operation>
class binder2nd : public unary_function<typename Operation::first_argument_type,
                                        typename Operation::result_type>
{
public:

    binder2nd(const Operation& x,const typename Operation::second_argument_type& y)
        :op(x),value(y){}

    typename Operation::result_type
    operator()(const typename Operation::first_argument_type& x) const
    {
        return op(value,x);
    }

private:

    Operation op;
    typename Operation::second_argument_type value;
};


template <class Operation,class T>
binder2nd<Operation> bind2nd(const Operation& op,const T& x)
{
    typename Operation::second_argument_type value=x;
    return binder1st<Operation>(op,value);
}


template <class Operation1,class Operation2>
class unary_compose : public unary_function<typename Operation2::argument_type,
                                            typename Operation1::result_type>
{
public:

    unary_compose(const Operation1& x,const Operation2& y):op1(x),op2(y){}

    typename Operation1::result_type
    operator()(const typename Operation2::argument_type& x)
    {
        return op1(op2(x));
    }

private:

    Operation1 op1;
    Operation2 op2;
};


template <class Operation1,class Operation2>
unary_compose<Operation1,Operation2>
compose1(const Operation1& x,const Operation2& y)
{
    return unary_compose<Operation1,Operation2>(x,y);
}


template <class Operation1,class Operation2,class Operation3>
class binary_compose : public unary_function<typename Operation2::argument_type,
                                             typename Operation1::result_type>
{
public:

    binary_compose(const Operation1& x,const Operation2& y,const Operation3& z)
        :op1(x),op2(y),op3(z){}

    typename Operation1::result_type
    operator()(const typename Operation2::argument_type& x)
    {
        return op1(op2(x),op3(x));
    }

private:

    Operation1 op1;
    Operation2 op2;
    Operation3 op3;

};


//wrap unary function ptr
template <class Operation1,class Operation2,class Operation3>
binary_compose<Operation1,Operation2,Operation3>
compose2(const Operation1& op1,const Operation2& op2,const Operation3& op3)
{
    return binary_compose<Operation1,Operation2,Operation3>(op1,op2,op3);
}


template <class Arg,class Result>
class pointer_to_unary_function : public unary_function<Arg,Result>
{
public:

    explicit pointer_to_unary_function(Result (*x)(Arg)):ptr(x){}

    Result operator()(Arg x) const {return ptr(x);}

private:

    Result (*ptr)(Arg);
};


template <class Arg,class Result>
pointer_to_unary_function<Arg,Result>
ptr_fun(Result (*x)(Arg))
{
    return pointer_to_unary_function<Arg,Result>(x);
}


//wrap binary function ptr
template <class Arg1,class Arg2,class Result>
class pointer_to_binary_function : public binary_function<Arg1,Arg2,Result>
{
public:

    explicit pointer_to_binary_function(Result (*x)(Arg1,Arg2)):ptr(x){}

    Result operator()(Arg1 x,Arg2 y) const {return ptr(x,y);}

private:

    Result (*ptr)(Arg1,Arg2);
};


template <class Arg1,class Arg2,class Result>
pointer_to_binary_function<Arg1,Arg2,Result>
ptr_fun(Result (*x)(Arg1,Arg2))
{
    return pointer_to_binary_function<Arg1,Arg2,Result>(x);
}


//member function
//no argument,by pointer,non-const
template <class R,class T>
class mem_fun_t : public unary_function<T*,R>
{
public:

    explicit mem_fun_t(R (T::*pf)()):f(pf){}

    R operator()(T* p) const {return (p->*f)();}

private:

    R (T::*f)();
};


//no argument,by pointer,const
template <class R,class T>
class const_mem_fun_t : public unary_function<const T*,R>
{
public:

    explicit const_mem_fun_t(R (T::*pf)() const):f(pf){}

    R operator()(const T* p) const {return (p->*f)();}

private:

    R (T::*f)() const;
};


//no argument,by reference,non-const
template <class R,class T>
class mem_fun_ref_t : public unary_function<T,R>
{
public:

    explicit mem_fun_ref_t(R (T::*pf)()):f(pf){}

    R operator()(T& r) const {return (r.*f)();}

private:

    R (T::*f)();
};


//no argument,by reference,const
template <class R,class T>
class const_mem_fun_ref_t : public unary_function<T,R>
{
public:

    explicit const_mem_fun_ref_t(R (T::*pf)() const):f(pf){}

    R operator()(const T& r) const {return (r.*f)();}

private:

    R (T::*f)() const;
};


//one argument,by pointer,non-const
template <class R,class T,class Arg>
class mem_fun1_t : public binary_function<T*,Arg,R>
{
public:

    explicit mem_fun1_t(R (T::*pf)(Arg)):f(pf){}

    R operator()(T* p,Arg x) const {return (p->*f)(x);}

private:

    R (T::*f)(Arg);
};


//one argument,by pointer,const
template <class R,class T,class Arg>
class const_mem_fun1_t : public binary_function<const T*,Arg,R>
{
public:

    explicit const_mem_fun1_t(R (T::*pf)(Arg) const):f(pf){}

    R operator()(const T* p,Arg x) const {return (p->*f)(x);}

private:

    R (T::*f)(Arg) const;
};


//one argument,by reference,non-const
template <class R,class T,class Arg>
class mem_fun1_ref_t : public binary_function<T,Arg,R>
{
public:

    explicit mem_fun1_ref_t(R (T::*pf)(Arg)):f(pf){}

    R operator()(T& r,Arg x) const {return (r.*f)(x);}

private:

    R (T::*f)(Arg);
};


//one argument,by reference,const
template <class R,class T,class Arg>
class const_mem_fun1_ref_t : public binary_function<T,Arg,R>
{
public:

    explicit const_mem_fun1_ref_t(R (T::*pf)(Arg) const):f(pf){}

    R operator()(const T& r,Arg x) const {return (r.*f)(x);}

private:

    R (T::*f)(Arg) const;
};


//
template <class R,class T>
mem_fun_t<R,T> mem_fun(R (T::*f)())
{
    return mem_fun_t<R,T>(f);
}

template <class R,class T>
const_mem_fun_t<R,T> mem_fun(R (T::*f)() const)
{
    return const_mem_fun_t<R,T>(f);
}

template <class R,class T>
mem_fun_ref_t<R,T> mem_fun_ref(R (T::*f)())
{
    return mem_fun_ref_t<R,T>(f);
}

template <class R,class T>
const_mem_fun_ref_t<R,T> mem_fun_ref(R (T::*f)() const)
{
    return const_mem_fun_ref_t<R,T>(f);
}

template <class R,class T,class Arg>
mem_fun1_t<R,T,Arg> mem_fun(R (T::*f)(Arg))
{
    return mem_fun1_t<R,T,Arg>(f);
}

template <class R,class T,class Arg>
const_mem_fun1_t<R,T,Arg> mem_fun(R (T::*f)(Arg) const)
{
    return const_mem_fun1_t<R,T,Arg>(f);
}

template <class R,class T,class Arg>
mem_fun1_ref_t<R,T,Arg> mem_fun_ref(R (T::*f)(Arg))
{
    return mem_fun1_ref_t<R,T,Arg>(f);
}

template <class R,class T,class Arg>
const_mem_fun1_ref_t<R,T,Arg> mem_fun_ref(R (T::*f)(Arg) const)
{
    return const_mem_fun1_ref_t<R,T,Arg>(f);
}



SSTL_NAMESPACE_END


#endif // FUNCTION_H_INCLUDED
