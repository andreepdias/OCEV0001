#include <stdio.h>
#include <stdlib.h>
using namespace std;

int sum(int a, int b){
    return a + b;
}

int s(int a, int b, int (*f) (int, int)){

    int x = 10, y = 20;

    int z = (*f)(x, y);

    return z;

}


int main(int argc, char const *argv[])
{
    
    cout << s (1, 2, sum) << endl;

    return 0;
}

