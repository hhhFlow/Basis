#include <iostream>
#include "insert_sort.h"
#include "bubble_sort.h"


int main(){

    int a[] = {654,17,39,21,5,1,2,8,99,1,45,34,66};

    bubble_sort(a,13);

    for(int i = 0; i<13;++i){
        std::cout<<" "<<a[i];
    }

    return 0;
}