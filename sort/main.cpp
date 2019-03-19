#include <iostream>
#include "insert_sort.h"



int main(){

    int a[] = {5,1,2,8,99,1,45,34,66};

    insert_sort(a,9);

    for(int i = 0; i<9;++i){
        std::cout<<" "<<a[i];
    }

    return 0;
}