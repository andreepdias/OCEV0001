#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;


int main()
{
    while(true){
        printf("validas numero invalidas distancia\n");
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        int celulas_validas = a, numero_movimentos = b, celulas_invalidas = c, distancia = d;
        double f = (double(celulas_validas) / (numero_movimentos + celulas_invalidas)) * (1.0 - (double(distancia) / 55));
        cout << f << endl;
    }

}