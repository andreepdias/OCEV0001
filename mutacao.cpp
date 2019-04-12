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

void Populacao::bit_flip(double probabilidade){
    uniform_real_distribution<double> distribution_real{0.0, 1.0};
    double r;
    for(int i = 0; i < tamanho_populacao; i++){
        // printf("i: %d\n", i);
        for(int j = 0; j < tamanho_cromossomo; j++){
            // printf("j: %d\n", j);
            r = distribution_real(engine);
            if(r <= probabilidade){
                (*individuos_binario)[i][j] = !((*individuos_binario)[i][j]);
            }
        }
    }
}

void Populacao::mutacao_delta(double limite_inferior, double limite_superior, int denominador = 10){
    uniform_real_distribution<double> distribution_real{limite_inferior, limite_superior};
    uniform_int_distribution<int> distribution_int{0, 1};
    double r;
    for(int i = 0; i < tamanho_populacao; i++){
        // printf("i: %d\n", i);
        for(int j = 0; j < tamanho_cromossomo; j++){
            // printf("j: %d\n", j);
            int soma = distribution_int(engine);
            double modificacao = distribution_real(engine)/denominador;
            if(soma){
                (*individuos_real)[i][j] = min((*individuos_real)[i][j] + modificacao, limite_superior);
            }else{
                (*individuos_real)[i][j] = max((*individuos_real)[i][j] - modificacao, limite_inferior);
            }
        }
    }
}
