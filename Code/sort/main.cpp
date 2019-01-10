#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <time.h>
#include "thread_stack.h"
using namespace std;
int quickSort(vector<int> &data, int left, int right, int depth) {
    const size_t data_size = right - left;
    int i = left, j = right;
    int tmp;
    if (data_size <= 1)
        return 0;

    int m = data[left + (right-left)/2];
    while (i <= j) {
        while (data[i] < m)
            i++;
        while (data[j] > m)
            j--;
        if (i <= j) {
            tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
            i++;
            j--;
        }
    };
    future<int> f;
    if (depth < 10) {
        if (left < j)
            f = async(std::launch::async, quickSort, std::ref(data), std::ref(left), std::ref(j), 1+depth);
        if (i < right)
            f = async(std::launch::async, quickSort, std::ref(data), std::ref(i), std::ref(right), 1+depth);
    } else {
        if (left < j)
            quickSort(data, left, j, 1+depth);
        if (i < right)
            quickSort(data, i, right, 1+depth);
    }
    return 0;
}
void quick_sort(std::vector<int>& arr) {
    quickSort(arr, 0, arr.size()-1, 0);
}
int quickSort2(vector<int> &data, int left, int right) {
    const size_t data_size = right - left;
    int i = left, j = right;
    int tmp;
    if (data_size <= 1)
        return 0;

    int m = data[left + (right-left)/2];
    while (i <= j) {
        while (data[i] < m)
            i++;
        while (data[j] > m)
            j--;
        if (i <= j) {
            tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
            i++;
            j--;
        }
    };

    if (left < j)
        quickSort2(data, left, j);
    if (i < right)
        quickSort2(data, i, right);
    return 0;
}

void quick_sort2(std::vector<int>& arr) {
    quickSort2(arr, 0, arr.size()-1);
}
void init(vector<int> &arr){
    for(int i=0; i<arr.size(); i++)
        arr[i] = rand()%100;
}
void print_vector(const vector<int> &arr){
    for(int i=0; i<arr.size(); i++)
        cout << arr[i] << " ";
    cout << endl;
}
bool check_qsort(const vector<int> &a, const vector<int> &b){
    for(int i=0; i<a.size(); i++){
        if(a[i] != b[i])
            return false;
    }
    return true;
}
int main() {
    int n = 100000000;
    vector<int> data1(n, 0);
    vector<int> data2(n, 0);
    init(data1);
    data2 = data1;

    auto start = std::chrono::high_resolution_clock::now();
    quick_sort(data2);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Time taken with maximum threads: " << elapsed.count() << endl;

    start = std::chrono::high_resolution_clock::now();
    quick_sort2(data1);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "Time taken with one thread: " <<  elapsed.count() << endl;
    cout << "Is true " << check_qsort(data1, data2);
    return 0;
}