#ifndef __DIVERSIDADE_CPP
#define __DIVERSIDADE_CPP

#include "populacao.cpp"
using namespace std;


void Dominio_Inteiro_Permutado::calcula_diversidade()
{
    vector<double> centroides(tamanho_cromossomo);

    for(int i = 0; i < tamanho_cromossomo; i++){
        for(int j = 0; j < tamanho_populacao; j++){
            centroides[i] += (*individuos)[j][i];
        }
        centroides[i] /= tamanho_populacao;
    }
    
    (*diversidade) = 0;
    for(int i = 0; i < tamanho_cromossomo; i++){
        for(int j = 0; j < tamanho_populacao; j++){
            (*diversidade) += pow(((*individuos)[j][i] - centroides[i]), 2);
        }
    }
}



#endif