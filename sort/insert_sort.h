
template <typename T>
void insert_sort(T array[], int n){
    T key;
    int pre;
    for(int i = 1; i < n; ++i){
        key = array[i];
        pre = i - 1;
        while((array[pre] > key )&& (pre >=0)){
            array[pre+1] = array[pre];
            pre--;
        }
        array[pre+1] = key;
    }
}
