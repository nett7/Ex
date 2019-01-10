#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <time.h>
using namespace std;

int min(int a, int b){
    return a < b ? a : b;
}
int multiply_vectors(const vector<vector<int> > &a, const vector<vector<int> > &b,
                     int row, int col){
    int sum = 0;
    for(int i=0; i<a[row].size(); i++){
        sum += a[row][i] * b[i][col];
    }
    return sum;
}
void print_vector(const vector<vector<int> > &v){
    for(int i=0; i<v.size(); i++){
        for(int j=0; j<v[i].size(); j++){
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}
void init(vector<vector<int> > &v){
    for(int i=0; i<v.size(); i++){
        for(int j=0; j<v[i].size(); j++){
            v[i][j] = rand()%10;
        }
    }
}
vector<vector<int> > multiply_matrix_one_thread(const vector<vector<int> > &a, const vector<vector<int> > &b){
    int n = a.size();
    int m = a[0].size();
    vector<vector<int> > c(n, vector<int>(m, 0));
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            c[i][j] = multiply_vectors(a, b, i, j);
        }
    }
    return c;
}

bool multiply_cols(const vector<vector<int> > &a, const vector<vector<int> > &b,
                   vector<vector<int> > &c, int l, int r){
    int n = a.size();
    int k = b[0].size();
    for(int i=l; i<r; i++){
        for(int j=0; j<k; j++){
            c[i][j] = multiply_vectors(a, b, i, j);
        }
    }
    return true;
}
vector<vector<int> > multiply_matrix_maximum_thread(const vector<vector<int> > &a, const vector<vector<int> > &b){
    int n = a.size();
    int count_of_thread = min(thread::hardware_concurrency(), n);
    int block = n/count_of_thread;
    int k;
    vector<vector<int> > c(n, vector<int>(n, 0));
    vector<future<bool> > f;
    for(int i=0; i<count_of_thread; i++){
        if(i != count_of_thread-1)
            k = i*block+block;
        else
            k = n;
        f.push_back(async(std::launch::async, multiply_cols, std::ref(a), std::ref(b), std::ref(c), i*block, k));
    }
    return c;
}
bool check(const vector<vector<int> > &a, const vector<vector<int> > &b){
    for(int i=0; i<a.size(); i++){
        for(int j=0; j<a[0].size(); j++){
            if(a[i][j] != b[i][j])
                return false;
        }
    }
    return true;
}
int main() {
    int n = 150;
    vector<vector<int> > a(n, vector<int>(n, 0));
    vector<vector<int> > b(n, vector<int>(n, 0));
    vector<vector<int> > c1;
    vector<vector<int> > c2;
    init(a);
    init(b);
    auto start = std::chrono::high_resolution_clock::now();
    c2 = multiply_matrix_maximum_thread(a, b);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Time taken with maximum threads: " << elapsed.count() << endl;

    start = std::chrono::high_resolution_clock::now();
    c1 = multiply_matrix_one_thread(a, b);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "Time taken with one thread: " <<  elapsed.count() << endl;

    cout << "Is true: " << check(c1, c2);
    return 0;
}