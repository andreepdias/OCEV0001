#ifndef __MUTACAO_CPP
#define __MUTACAO_CPP

#include "populacao.cpp"
using namespace std;

void Dominio_Inteiro_Permutado::swap_mutation()
{
    uniform_real_distribution<double> distribution_real{0.0, 1.0};
    uniform_int_distribution<int> distribution_int(0, tamanho_cromossomo - 1);


    #pragma omp parallel
    #pragma omp for schedule(dynamic)
    for(int i = 0; i < tamanho_populacao; i++){
        double r;
        int g;

        for(int j = 0; j < tamanho_cromossomo; j++){
            r = distribution_real(engine);
            // printf("\nMutacao: %lf\tr: %lf\n", probabilidade_mutacao * 100, r * 100);
            if(r < probabilidade_mutacao){
                // printf("r < mutacao, entrou.\n");
                do{
                    g = distribution_int(engine);
                }while(g == j);
                // printf("j: %d\tg: %d\n", j, g);
                int aux = (*individuos)[i][j];
                (*individuos)[i][j] = (*individuos)[i][g];
                (*individuos)[i][g] = aux;
            }
        }
    }

}

void Populacao::bit_flip(){
    uniform_real_distribution<double> distribution_real{0.0, 1.0};
    double r;
    for(int i = 0; i < tamanho_populacao; i++){
        // printf("i: %d\n", i);
        for(int j = 0; j < tamanho_cromossomo; j++){
            // printf("j: %d\n", j);
            r = distribution_real(engine);
            if(r <= probabilidade_mutacao){
                (*individuos_binario)[i][j] = !((*individuos_binario)[i][j]);
            }
        }
    }
}
/*
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
*/

#endif