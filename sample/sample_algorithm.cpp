
#include "util.h"
//#include <algorithm>
#include "../include/utilities.h"

#define CONTAINER_INT VECTOR_INT

using namespace sstl;

int main()
{
    VECTOR_INT  v1{1,2,3,4};
    VECTOR_INT  v2;
    LIST_INT    li1{1,2,3,4};
    DEQUE_INT   de1{1,2,3,4};

    //equal
    assert(equal(v1.begin(),v1.end(),li1.begin()));
    assert(equal(v1.begin(),v1.end(),de1.begin()));
    assert(equal(li1.begin(),li1.end(),de1.begin()));

    println("fill");
    fill(v1.begin(),v1.end(),5);PRINT(v1);

    println("fill");
    fill_n(li1.begin(),3,-1);PRINT(li1);

    println("iter_swap");
    iter_swap(v1.begin(),li1.begin());PRINT(v1);PRINT(li1);

    //lexicographical_compare
    v1={1,2,3,4};li1={1,2,3,4,5};de1={1,2,3};
    assert(lexicographical_compare(v1.begin(),v1.end(),li1.begin(),li1.end()));
    assert(!lexicographical_compare(v1.begin(),v1.end(),de1.begin(),de1.end()));

    println("mismatch");
    v1={1,2,3,4,6};li1={1,2,3,4,5};
    auto p=mismatch(v1.begin(),v1.end(),li1.begin());
    println(*(p.first),*(p.second));

    println("swap");
    v1={1,2,3,4,5};v2={5,4,3,2,1};
    swap(v1,v2);PRINT(v1);PRINT(v2);

    //adjacent_find
    assert(adjacent_find(v1.begin(),v1.end())==v1.end());

    //count
    assert(count(v1.begin(),v1.end(),6)==0);

    //count_if
    assert(count_if(v1.begin(),v1.end(),[](const int& a){return a<5;})==4);

    //find
    assert(*find(v1.begin(),v1.end(),4)==4);

    //find_if
    assert(*find_if(v1.begin(),v1.end(),[](const int& a){return a>=5;})==5);


    //includes
    v1={1,2,3,4,5};v2={1,3,4};
    assert(includes(v1.begin(),v1.end(),v2.begin(),v2.end()));

    //max_element min_element
    assert(*max_element(v1.begin(),v1.end())==5);
    assert(*min_element(v1.begin(),v1.end())==1);

    //merge
    println("\nmerge");
    VECTOR_INT v_merge(10);
    auto end=merge(v1.begin(),v1.end(),v2.begin(),v2.end(),v_merge.begin());
    for_each(v_merge.begin(),end,[](const int& a){printc(a," ");});

    //partition
    println("\npartition");
    v1={1,2,-1,4,-2,1,7,-1,-9};
    partition(v1.begin(),v1.end(),[](const int& a){return a<0;});PRINT(v1);

    //remove
    println("\nremove");
    remove(v1.begin(),v1.end(),-1);PRINT(v1);

    //remove_if
    println("remove_if");
    remove_if(v1.begin(),v1.end(),[](const int& a){return a<0;});PRINT(v1);

    //replace
    println("replace");
    replace(v1.begin(),v1.end(),2,0);PRINT(v1);

    //replace_if
    println("replace_if");
    replace_if(v1.begin(),v1.end(),[](const int& a){return a<3;},3);PRINT(v1);

    //reverse
    println("reverse");
    reverse(v1.begin(),v1.end());PRINT(v1);

    //rotate
    println("rotate");v1={1,2,3,4,5,6,7,8};
    rotate(v1.begin(),advance(v1.begin(),2),v1.end());PRINT(v1);

    //search
    v1={3,4,3,4,5,6,5},v2={3,4,5};
    assert(search(v1.begin(),v1.end(),v2.begin(),v2.end())==advance(v1.begin(),2));

    //search_n
    assert(search_n(v1.begin(),v1.end(),3,3,[](const int& a,const int& b){return a>b;})==advance(v1.begin(),3));

    //unique
    println("unique");v1={1,1,2,3,3,3,4,5,5,6,7};
    for_each(v1.begin(),unique(v1.begin(),v1.end()),print_single<int>());


    //lower_bound
    v1={1,1,2,3,3,4,7,8};
    assert(lower_bound(v1.begin(),v1.end(),3)==advance(v1.begin(),3));
    assert(lower_bound(v1.begin(),v1.end(),6)==advance(v1.begin(),6));
    assert(lower_bound(v1.begin(),v1.end(),9)==v1.end());

    //upper_bound
    assert(upper_bound(v1.begin(),v1.end(),3)==advance(v1.begin(),5));
    assert(upper_bound(v1.begin(),v1.end(),6)==advance(v1.begin(),6));
    assert(lower_bound(v1.begin(),v1.end(),9)==v1.end());

    //binary_search
    assert(binary_search(v1.begin(),v1.end(),3));
    assert(!binary_search(v1.begin(),v1.end(),6));
    assert(!binary_search(v1.begin(),v1.end(),9));

    //next_permutation prev_permutation
    println("next_permutation");v1={2,1,3,5,4};
    next_permutation(v1.begin(),v1.end());PRINT(v1);
    prev_permutation(v1.begin(),v1.end());PRINT(v1);

    //random_shuffle
    println("random_shuffle");v1={1,2,3,4,5,6,7};
    random_shuffle(v1.begin(),v1.end());PRINT(v1);

    //sort
    println("sort");
    sort(v1.begin(),v1.end());PRINT(v1);

    //partial_sort
    println("partial_sort");v1={1,3,7,6,3,4,2};
    partial_sort(v1.begin(),advance(v1.begin(),4),v1.end());PRINT(v1);

    //algorithms for set
    set<int> s1{2,3,4,1,7,8};
    set<int> s2{1,5,6,8,9,4};
    vector<int> set_result;

    println("set_union");
    set_union(s1.begin(),s1.end(),s2.begin(),s2.end(),back_inserter(set_result));
    PRINT(set_result);set_result.clear();
    println("set_intersection");
    set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(),back_inserter(set_result));
    PRINT(set_result);set_result.clear();
    println("set_difference");
    set_difference(s1.begin(),s1.end(),s2.begin(),s2.end(),back_inserter(set_result));
    PRINT(set_result);set_result.clear();
    println("set_symmetric_difference");
    set_symmetric_difference(s1.begin(),s1.end(),s2.begin(),s2.end(),back_inserter(set_result));
    PRINT(set_result);set_result.clear();


    return 0;
}
