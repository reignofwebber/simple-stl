#include "utility.h"

#include "../include/numeric.h"

#define CONTAINER_INT VECTOR_INT

using namespace sstl;

int main()
{

    CONTAINER_INT con{5,2,1,4,5};
    CONTAINER_INT con2{1,2,3,4,5};

    CONTAINER_INT result(5);


    println("accumulate : ",accumulate(con.begin(),con.end(),0));

    println("\nadjacent_difference : ");
    adjacent_difference(con.begin(),con.end(),result.begin());
    PRINT(result);

    println("\ninner_product : ",inner_product(con.begin(),con.end(),con2.begin(),0));

    println("\npartial_sum :");
    partial_sum(con2.begin(),con2.end(),result.begin());
    PRINT(result);


    return 0;
}
