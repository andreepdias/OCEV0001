#include <bits/stdc++.h>
using namespace std;

int main(){
    random_device device{};
    mt19937 engine{device()};
    // uniform_real_distribution<double> distribution{0.0, 1.0};
    uniform_int_distribution<int> distribution(0, 10);

    int r;
    do{
        r = distribution(engine);
        cout << r << endl;
    }while(r != 10);
    
    return 0;   
}