#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;


int main()
{

    double x = 10.0;
    double a = 0.0, b = 1.0, c = 0.0;

    for(int i = 1; i <= 50; i++){
        a += pow(cos(0), 4);
    }
    for(int i = 1; i <= 50; i++){
        b *= pow(cos(0), 2);
    }
    b *= 2;

    for(int i = 1; i <= 50; i++){
        c += i * pow(0, 2);
    }
    c = sqrt(c);
    if(c == 0) c = DBL_EPSILON;

    double f = abs(a - b) / c;

    printf("a: %lf b: %lf c: %lf\tfx: %lf\n", a, b, c, f);

    double r1 = 1.0;
    for(int i = 1; i <= 50; i++){
        r1 *= x;
    }

    printf("r1: %lf\n", r1);

    double r2 = 0.0;
    for(int i = 1; i <= 50; i++){
        r2 += x;
    }

    printf("r2: %lf\n", r2);


}