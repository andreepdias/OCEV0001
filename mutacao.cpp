#include <bits/stdc++.h>
#include "populacao.cpp"
using namespace std;

void Populacao::swap_mutation(double probabilidade){
    uniform_real_distribution<double> distribution_real{0.0, 1.0};
    uniform_int_distribution<int> distribution_int(0, tamanho_cromossomo - 1);
    double r;
    int g;
    for(int i = 0; i < tamanho_populacao; i++){
        // printf("i: %d\n", i);
        for(int j = 0; j < tamanho_cromossomo; j++){
            // printf("j: %d\n", j);   
            r = distribution_real(engine);
            if(r <= probabilidade){
                do{
                    g = distribution_int(engine);
                }while(g == j);
                // printf("i: %d\tj: %d\n", i, j);
                int aux = (*individuos_inteiro_permutado)[i][j];
                (*individuos_inteiro_permutado)[i][j] = (*individuos_inteiro_permutado)[i][g];
                (*individuos_inteiro_permutado)[i][g] = aux;
            }
        }
    }

}

