#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

#include "iterator_traits.h"
#include "iterator.h"
#include "utilities.h"


#define USE_RECUR 0

SSTL_NAMESPACE_BEGIN

//sequence
//equal
template <class InputIterator1,class InputIterator2>
bool equal(InputIterator1 begin1,InputIterator1 end1,
           InputIterator2 begin2)
{
    for(;begin1!=end1;++begin1,++begin2)
        if(*begin1!=*begin2)
            return false;
    return true;
}


//fill
template <class ForwardIterator,class T>
void fill(ForwardIterator begin,ForwardIterator end,const T& value)
{
    for(;begin!=end;++begin)
        *begin=value;
}


//fill_n
template <class OutputIterator,class Size,class T>
OutputIterator fill_n(OutputIterator begin,Size n,const T& value)
{
    for(;n>0;--n,++begin)
        *begin=value;
    return begin;
}


//iter_swap
template <class ForwardIterator1,class ForwardIterator2>
void iter_swap(ForwardIterator1 iter1,ForwardIterator2 iter2)
{
    auto tmp=*iter1;*iter1=*iter2;*iter2=tmp;
}


//lexicographical_compare
template <class InputIterator1,class InputIterator2>
bool lexicographical_compare(InputIterator1 begin1,InputIterator1 end1,
                             InputIterator2 begin2,InputIterator2 end2)
{
    for(;begin1!=end1 && begin2!=end2;++begin1,++begin2){
        if(*begin1<*begin2)
            return true;
        else if(*begin1 >*begin2)
            return false;
    }
    return begin1==end1 && begin2!=end2;
}


//max
template <class T>
const T& max(const T& a,const T& b)
{
    return a < b ? b : a;
}


//min
template <class T>
const T& min(const T& a,const T& b)
{
    return a < b ? a : b;
}


//mismatch
template <class InputIterator1,class InputIterator2>
pair<InputIterator1,InputIterator2>
mismatch(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2)
{
    for(;begin1!=end1 && *begin1==*begin2;++begin1,++begin2)
        ;
    return pair<InputIterator1,InputIterator2>(begin1,begin2);
}


//swap
template <class T>
void swap(T& a,T& b)
{
    auto tmp=a;a=b;b=tmp;
}


//copy--to be perfected
template <class InputIterator,class OutputIterator>
OutputIterator copy(InputIterator begin,InputIterator end,
                    OutputIterator result)
{
    for(;begin!=end;++begin,++result)
        *result=*begin;
    return result;
}


//copy_backward--to be perfected
template <class BidirectionalIterator1,class BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 begin,
                                     BidirectionalIterator1 end,
                                     BidirectionalIterator2 result)
{
    auto rbegin=reverse_iterator<BidirectionalIterator1>(end);
    auto rend=reverse_iterator<BidirectionalIterator1>(begin);

    for(;rbegin!=rend;++rbegin,--result)
        *result=*rbegin;

    return result;
}


//other
//adjacent_find
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator begin,ForwardIterator end)
{
    if(begin==end) return end;
    auto pre=begin++;
    while(begin!=end){
        if(*pre==*begin) return pre;
        pre=begin++;
    }
    return end;
}


//count
template <class InputIterator,class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator begin,InputIterator end,const T& value)
{
    typename iterator_traits<InputIterator>::difference_type n=0;
    while(begin!=end)
        if(*begin++==value)
            ++n;
    return n;
}


//count_if
template <class InputIterator,class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator begin,InputIterator end,Predicate pred)
{
    typename iterator_traits<InputIterator>::difference_type n=0;
    while(begin!=end)
        if(pred(*begin++))
            ++n;
    return n;
}


//find
template <class InputIterator,class T>
InputIterator find(InputIterator begin,InputIterator end,const T& value)
{
    while(begin!=end && *begin!=value)
        ++begin;
    return begin;
}


//find_if
template <class InputIterator,class Predicate>
InputIterator find_if(InputIterator begin,InputIterator end,Predicate pred)
{
    while(begin!=end && !pred(*begin))
        ++begin;
    return begin;
}


//find_end--to do
template <class ForwardIterator1,class ForwardIterator2>
ForwardIterator1 find_end(ForwardIterator1 begin1,ForwardIterator1 end1,
                          ForwardIterator2 begin2,ForwardIterator2 end2)
{

}


//find_first_of
template <class InputIterator,class ForwardIterator>
InputIterator find_first_of(InputIterator begin1,InputIterator end1,
                            ForwardIterator begin2,ForwardIterator end2)
{
    for(;begin1!=end1;++begin1)
        for(auto iter=begin2;iter!=end2;++iter)
            if(*begin1==*iter)
                return begin1;
    return end1;
}


//for_each
template <class InputIterator,class Function>
Function for_each(InputIterator begin,InputIterator end,Function func)
{
    for(;begin!=end;++begin)
        func(*begin);
    return func;
}


//generate
template <class ForwardIterator,class Generator>
void generate(ForwardIterator begin,ForwardIterator end,Generator gen)
{
    for(;begin!=end;++begin)
        *begin=gen();
}


//generate_n
template <class OutputIterator,class Size,class Generator>
OutputIterator generate_n(OutputIterator begin,Size n,Generator gen)
{
    for(;n>0;--n,++begin)
        *begin=gen();
    return begin;
}


//includes
template <class InputIterator1,class InputIterator2>
bool includes(InputIterator1 begin1,InputIterator1 end1,
              InputIterator2 begin2,InputIterator2 end2)
{
    while(begin1!=end1 && begin2!=end2)
        if(*begin1>*begin2)
            return false;
        else if(*begin1<*begin2)
            ++begin1;
        else
            ++begin1,++begin2;
    return begin2==end2;
}


//max_element
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator begin,ForwardIterator end)
{
    if(begin==end) return end;
    auto result=begin;
    while(++begin!=end)
        if(*begin>*result)
            result=begin;
    return result;
}


//min_element
template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator begin,ForwardIterator end)
{
    if(begin==end) return end;
    auto result=begin;
    while(++begin!=end)
        if(*begin<*result)
            result=begin;
    return result;
}


//merge
template <class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator merge(InputIterator1 begin1,InputIterator1 end1,
                     InputIterator2 begin2,InputIterator2 end2,
                     OutputIterator result)
{
    while(begin1!=end1 && begin2!=end2)

        if(*begin1<*begin2)
            *result++=*begin1++;
        else
            *result++=*begin2++;

    return copy(begin1,end1,copy(begin2,end2,result));
}


//partition
template <class BidirectionalIterator,class Predicate>
BidirectionalIterator partition(BidirectionalIterator begin,
                                BidirectionalIterator end,
                                Predicate pred)
{
    while(true){
        while(true)
            if(begin==end)
                return begin;
            else if(pred(*begin))
                ++begin;
            else
                break;
        --end;
        while(true)
            if(begin==end)
                return begin;
            else if(!pred(*end))
                --end;
            else
                break;
        iter_swap(begin,end);
        ++begin;
    }
}


//remove_copy
template <class InputIterator,class OutputIterator,class T>
OutputIterator remove_copy(InputIterator begin,InputIterator end,
                           OutputIterator result,const T& value)
{
    for(;begin!=end;++begin)
        if(*begin!=value)
            *result++=*begin;
    return result;
}


//remove_copy_if
template <class InputIterator,class OutputIterator,class Predicate>
OutputIterator remove_copy_if(InputIterator begin,InputIterator end,
                           OutputIterator result,Predicate pred)
{
    for(;begin!=end;++begin)
        if(!pred(*begin))
            *result++=*begin;
    return result;
}


//remove
template <class ForwardIterator,class T>
ForwardIterator remove(ForwardIterator begin,ForwardIterator end,const T& value)
{
    begin=find(begin,end,value);    //for the worst case.
    auto next=begin;
    return begin==end ? end : remove_copy(++next,end,begin,value);
}


//remove_if
template <class ForwardIterator,class Predicate>
ForwardIterator remove_if(ForwardIterator begin,ForwardIterator end,Predicate pred)
{
    begin=find_if(begin,end,pred);    //for the worst case.
    auto next=begin;
    return begin==end ? end : remove_copy_if(++next,end,begin,pred);
}


//replace
template <class ForwardIterator,class T>
void replace(ForwardIterator begin,ForwardIterator end,
             const T& old_value,const T& new_value)
{
    for(;begin!=end;++begin)
        if(*begin==old_value)
            *begin=new_value;
}


//replace_if
template <class ForwardIterator,class Predicate,class T>
void replace_if(ForwardIterator begin,ForwardIterator end,
             Predicate pred,const T& new_value)
{
    for(;begin!=end;++begin)
        if(pred(*begin))
            *begin=new_value;
}


//replace_copy
template <class InputIterator,class OutputIterator,class T>
OutputIterator replace_copy(InputIterator begin,InputIterator end,
                            OutputIterator result,
                            const T& old_value,const T& new_value)
{
    for(;begin!=end;++begin,++result)
        *result= *begin==old_value ? new_value : *begin;
    return result;
}


//replace_copy_if
template <class InputIterator,class OutputIterator,class Predicate,class T>
OutputIterator replace_copy_if(InputIterator begin,InputIterator end,
                            OutputIterator result,
                            Predicate pred,const T& new_value)
{
    for(;begin!=end;++begin,++result)
        *result= pred(*begin) ? new_value : *begin;
    return result;
}


//reverse--to be perfected
template <class BidirectionalIterator>
void reverse(BidirectionalIterator begin,BidirectionalIterator end)
{
    while(begin!=end && begin!=--end)
        iter_swap(begin++,end);
}


//reverse_copy
template <class BidirectionalIterator,class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator begin,
                            BidirectionalIterator end,
                            OutputIterator result)
{
    while(begin!=end)
        *result++=*--end;
    return result;
}


//rotate--to be perfected
template <class ForwardIterator>
void rotate(ForwardIterator begin,ForwardIterator middle,ForwardIterator end)
{
    if(begin==middle || end==middle) return;
    auto iter=middle;
    while(true){
        iter_swap(begin++,iter++);
        if(begin==middle && iter==end)
            return;
        else if(begin==middle)
            middle=iter;
        else if(iter==end)
            iter=middle;
    }
}


//rotate_copy
template <class ForwardIterator,class OutputIterator>
OutputIterator rotate_copy(ForwardIterator begin,ForwardIterator middle,
                           ForwardIterator end,OutputIterator result)
{
    return copy(begin,middle,copy(middle,end,result));
}


//search
template <class ForwardIterator1,class ForwardIterator2>
ForwardIterator1 search(ForwardIterator1 begin1,ForwardIterator1 end1,
                        ForwardIterator2 begin2,ForwardIterator2 end2)
{

    auto d1=distance(begin1,end1);
    auto d2=distance(begin2,end2);

    if(d1<d2) return end1;
    auto iter1=begin1;
    auto iter2=begin2;
    while(iter2!=end2){
        if(*iter1==*iter2)
            ++iter1,++iter2;
        else
            if(d1==d2)
                return end1;
            else{
                iter1=++begin1;
                iter2=begin2;
                --d1;
            }
    }
    return begin1;
}


//search_n
template <class ForwardIterator,class Size,class T>
ForwardIterator search_n(ForwardIterator begin,ForwardIterator end,
                         Size count,const T& value)
{
    if(count<1) return begin;

    while((begin=find(begin,end,value)!=end)){
        auto iter=begin;
        while(--count && ++iter!=end && *iter==value)
            ;
        if(!count)
            return begin;
        begin=iter;
    }
    return end;
}


//search_n
template <class ForwardIterator,class Size,class T,class BinaryPredicate>
ForwardIterator search_n(ForwardIterator begin,ForwardIterator end,
                         Size count,const T& value,BinaryPredicate binary_pred)
{
    if(count<1) return begin;

    while((begin=find_if(begin,end,[value,binary_pred](const T& a){return binary_pred(a,value);}))!=end){
        auto iter=begin;
        while(--count && ++iter!=end && binary_pred(*iter,value))
            ;
        if(!count)
            return begin;
        begin=iter;
    }
    return end;
}


//swap_ranges
template <class ForwardIterator1,class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 begin1,ForwardIterator1 end1,
                             ForwardIterator1 begin2)
{
    for(;begin1!=end1;++begin1,++begin2)
        iter_swap(begin1,begin2);
    return begin2;
}


//transform
template <class InputIterator,class OutputIterator,class UnaryOperation>
OutputIterator transform(InputIterator begin,InputIterator end,
                         OutputIterator result,UnaryOperation op)
{
    for(;begin!=end;++begin,++result)
        *result=op(*begin);
    return result;
}


//transform
template <class InputIterator1,class InputIterator2,class OutputIterator,class BinaryOperation>
OutputIterator transform(InputIterator1 begin1,InputIterator1 end1,
                         InputIterator2 begin2,OutputIterator result,BinaryOperation binary_op)
{
    for(;begin1!=end1;++begin1,++begin2,++result)
        *result=binary_op(*begin1,*begin2);
}


//unique_copy
template <class InputIterator,class OutputIterator>
OutputIterator unique_copy(InputIterator begin,InputIterator end,OutputIterator result)
{
    *result=*begin++;
    for(;begin!=end;++begin)
        if(*result!=*begin)
            *++result=*begin;
    return ++result;
}


//unique
template <class ForwardIterator>
ForwardIterator unique(ForwardIterator begin,ForwardIterator end)
{
    return unique_copy(begin,end,begin);
}


//lower_bound(ordered)
template <class ForwardIterator,class T>
ForwardIterator lower_bound(ForwardIterator begin,ForwardIterator end,const T& value)
{
    auto len=distance(begin,end);
    decltype(len) half;
    decltype(begin) middle;
    while(len>0){
        half=len >> 1;
        middle=advance(begin,half);
        if(*middle<value){
            begin=middle;
            ++begin;
            len=len-half-1;
        }else{
            end=middle;     //can be deleted
            len=half;
        }
    }
    return begin;
}


//upper_bound
template <class ForwardIterator,class T>
ForwardIterator upper_bound(ForwardIterator begin,ForwardIterator end,const T& value)
{
    auto len=distance(begin,end);
    decltype(len) half;
    decltype(begin) middle;
    while(len>0){
        half=len >> 1;
        middle=advance(begin,half);
        if(*middle>value){
            end=middle;     //can be deleted
            len=half;
        }else{
            begin=middle;
            ++begin;
            len=len-half-1;
        }
    }
    return begin;
}


//equal_range
template <class ForwardIterator,class T>
pair<ForwardIterator,ForwardIterator>
equal_range(ForwardIterator begin,ForwardIterator end,const T& value)
{
    auto len=distance(begin,end);
    decltype(len) half;
    decltype(begin) middle,left,right;
    while(len>0){
        half=len >> 1;
        middle=advance(begin,half);
        if(*middle<value){
            begin=middle;
            ++begin;
            len=len-half-1;
        }else if(value<*middle){
            len=half;
        }else{
           left=lower_bound(begin,middle,value);
           right=upper_bound(++middle,advance(begin,len),value);
           return {left,right};
        }
    }
    return {begin,begin};
}


//binary_search
template <class ForwardIterator,class T>
bool binary_search(ForwardIterator begin,ForwardIterator end,const T& value)
{
    auto iter=lower_bound(begin,end,value);
    return iter!=end && *iter==value;   //
}


//next_permutation
template <class BidirectionalIterator>
bool next_permutation(BidirectionalIterator begin,BidirectionalIterator end)
{
    if(begin==end) return false;
    auto iter=end;
    if(--iter==begin) return false;
    decltype(begin) iter_next;
    while(true){
        iter_next=iter--;
        if(*iter<*iter_next){
            auto i=end;
            while(*--i<*iter);  //
            iter_swap(iter,i);
            reverse(iter_next,end);
            return true;
        }
        if(iter==begin){
            reverse(begin,end);
            return false;
        }
    }
}


//prev_permutation
template <class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator begin,BidirectionalIterator end)
{
    if(begin==end) return false;
    auto iter=end;
    if(--iter==begin) return false;
    decltype(begin) iter_next;
    while(true){
        iter_next=iter--;
        if(*iter_next<*iter){
            auto i=end;
            while(*iter<*--i);  //
            iter_swap(iter,i);
            reverse(iter_next,end);
            return true;
        }
        if(iter==begin){
            reverse(begin,end);
            return false;
        }
    }
}


//random_shuffle
template <class RandomAccessIterator>
void random_shuffle(RandomAccessIterator begin,RandomAccessIterator end)
{
    if(begin==end) return;
    for(auto iter=begin+1;iter!=end;++iter)
        iter_swap(iter,begin+(rand()%(iter-begin+1)));
}

//sort
//insertion_sort
template <class RandomAccessIterator>
void insertion_sort(RandomAccessIterator begin,
                    RandomAccessIterator end)
{
    if(begin==end) return;

    for(auto i=begin+1;i!=end;++i){
        for(auto j=begin;j!=i;++j){
            if(*j>*i){
                auto value=*i;
                copy_backward(j,i,i);
                *j=value;
            }
        }
    }
}


//quick_sort
//median
template <class T>
const T& median(const T& a,const & b,const & c)
{
    return a<b ? (b<c? b:(a<c? c : a)) : (a<c? a:(b<c? c : b));
}

template <class RandomAccessIterator,class T>
RandomAccessIterator sort_partition(RandomAccessIterator begin,
                                    RandomAccessIterator end,
                                    const T& pivot)
{
    while(true){
        while(*begin<pivot)
            ++begin;
        --end;
        while(pivot<*end)
            --end;
        if(!(begin<end))
            return begin;
        iter_swap(begin,end);
        ++begin;
    }
}

template <class RandomAccessIterator>
void sort(RandomAccessIterator begin,RandomAccessIterator end)
{
    if(end-begin>1){
        auto pivot_iter=sort_partition(begin,end,median(*begin,*(end-1),*(begin+(end-begin)/2)));
        sort(begin,pivot_iter);
        sort(pivot_iter,end);
    }
}


//push_heap
template <class RandomAccessIterator,class Offset,class T>
void do_push_heap(RandomAccessIterator begin,
                  Offset top,Offset hole,T value)
{
#if USE_RECUR
    Offset parent=(hole-1)/2;
    if(hole!=top && *(begin+parent)<value){
        *(begin+hole)=*(begin+parent);
        do_push_heap(begin,top,parent,value);
    }else
        *(begin+hole)=value;
#else
    Offset parent=(hole-1)/2;
    while(hole!=top && *(begin+parent)<value){
        *(begin+hole)=*(begin+parent);
        hole=parent;
        parent=(hole-1)/2;
    }
    *(begin+hole)=value;
#endif
}

template <class RandomAccessIterator,class Offset,class T>
void push_heap_dispatch(RandomAccessIterator begin,
                        RandomAccessIterator end,Offset*,T*)
{
    do_push_heap(begin,Offset(0),Offset(end-begin-1),T(*(end-1)));
}

template <class RandomAccessIterator>
void push_heap(RandomAccessIterator begin,RandomAccessIterator end)
{
    push_heap_dispatch(begin,end,distance_type(begin),value_type(begin));
}


//pop_heap
template <class RandomAccessIterator,class Offset,class T>
void do_adjust_heap(RandomAccessIterator begin,
                 Offset hole,Offset length,T value)
{
    Offset top=hole;    //for make_heap
    Offset child=2*hole+1;
    while(child+1<length){
        if(*(begin+child)<*(begin+(child+1)))
            ++child;
        *(begin+hole)=*(begin+child);
        hole=child;
        child=2*child+1;
    }
    if(child+1==length){
        *(begin+hole)=*(begin+child);
        hole=child;
    }
    //'*(begin+hole)=value' is wrong!!!
    //can not compare the last two nodes.
    do_push_heap(begin,top,hole,value);
}

template <class RandomAccessIterator,class Offset,class T>
void pop_heap_dispatch(RandomAccessIterator begin,
                       RandomAccessIterator end,
                       RandomAccessIterator result,Offset*,T*)
{
    T value=*result;*result=*begin;
    do_adjust_heap(begin,Offset(0),Offset(end-begin-1),value);
}


template <class RandomAccessIterator>
void pop_heap(RandomAccessIterator begin,RandomAccessIterator end)
{

    pop_heap_dispatch(begin,end,end-1,distance_type(begin),value_type(begin));
}


//sort heap
template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator begin,RandomAccessIterator end)
{
    while(end-begin>1)
        pop_heap(begin,end--);
}


//make heap
template <class RandomAccessIterator,class Offset,class T>
void do_make_heap(RandomAccessIterator begin,
                  RandomAccessIterator end,Offset*,T*)
{
    if(end-begin<2) return;
    Offset length=end-begin;

    Offset parent=(length-2)/2;
    while(true){
        do_adjust_heap(begin,parent,length,*(begin+parent));
        if(parent==0) break;
        --parent;
    }

}

template <class RandomAccessIterator>
void make_heap(RandomAccessIterator begin,RandomAccessIterator end)
{
    do_make_heap(begin,end,distance_type(begin),value_type(begin));
}


//partial_sort
template <class RandomAccessIterator>
void partial_sort(RandomAccessIterator begin,RandomAccessIterator middle,RandomAccessIterator end)
{
    make_heap(begin,middle);
    for(RandomAccessIterator iter=middle;iter!=end;++iter)
        if(*iter<*begin)
            pop_heap_dispatch(begin,middle,iter,distance_type(begin),value_type(begin));

    sort_heap(begin,middle);
}


//for set
//set_union
template <class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator set_union(InputIterator1 begin1,InputIterator1 end1,
                         InputIterator2 begin2,InputIterator2 end2,OutputIterator result)
{
    while(begin1!=end1 && begin2!=end2){
        if(*begin1<*begin2){
            *result=*begin1;
            ++begin1;
        }else if(*begin2<*begin1){
            *result=*begin2;
            ++begin2;
        }else{
            *result=*begin1;
            ++begin1;
            ++begin2;
        }
        ++result;
    }
    return copy(begin1,end1,copy(begin2,end2,result));
}


//set_intersection
template <class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator set_intersection(InputIterator1 begin1,InputIterator1 end1,
                                InputIterator2 begin2,InputIterator2 end2,OutputIterator result)
{
    while(begin1!=end1 && begin2!=end2){
        if(*begin1<*begin2){
            ++begin1;
        }else if(*begin2<*begin1){
            ++begin2;
        }else{
            *result=*begin1;
            ++begin1;
            ++begin2;
            ++result;
        }
    }
    return result;
}


//set_difference
template <class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator set_difference(InputIterator1 begin1,InputIterator1 end1,
                              InputIterator2 begin2,InputIterator2 end2,OutputIterator result)
{
    while(begin1!=end1 && begin2!=end2){
        if(*begin1<*begin2){
            *result=*begin1;
            ++begin1;
            ++result;
        }else if(*begin2<*begin1){
            *result=*begin2;
            ++begin2;
            ++result;
        }else{
            ++begin1;
            ++begin2;
        }
    }
    return copy(begin1,end1,result);
}


//set_symmetic_difference
template <class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 begin1,InputIterator1 end1,
                                        InputIterator2 begin2,InputIterator2 end2,OutputIterator result)
{
    while(begin1!=end1 && begin2!=end2){
        if(*begin1<*begin2){
            *result=*begin1;
            ++begin1;
            ++result;
        }else if(*begin2<*begin1){
            *result=*begin2;
            ++begin2;
            ++result;
        }else{
            ++begin1;
            ++begin2;
        }
    }
    return copy(begin1,end1,copy(begin2,end2,result));
}


SSTL_NAMESPACE_END

#endif // ALGORITHM_H_INCLUDED
