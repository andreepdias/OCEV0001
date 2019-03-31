#include <bits/stdc++.h>
using namespace std;

int main(){
    int tamanho_populacao = 10;

    random_device device{};
    mt19937 engine{device()};
    uniform_int_distribution<int> distribution(0, tamanho_populacao - 1);

    int d = 1;

    vector<int> individuos_aleatorios;
    for (int i = 0; i < tamanho_populacao; i++)
    {
        individuos_aleatorios[i] = i;
    }
    shuffle(individuos_aleatorios.begin(), individuos_aleatorios.end() - (tamanho_populacao / 2), engine);
    
    for(int i = 0; i < tamanho_populacao; i++){
        
    }

    return 0;   
}