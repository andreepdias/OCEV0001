#include <bits/stdc++.h>
#include "dominios.cpp"
using namespace std;


int Dominio_Binario::funcaoCOS(){

    double x, fx, max, min;
    int l = pow(2, 16) - 1, d;

    for(int i = 0; i < tamanho_populacao; i++){
        d = 0;
        for(int j = tamanho_cromossomo - 1; j >= 0; j--){
            d += pow(2, j) * (*individuos)[i][j];
        }
        x = (*limites)[0].first + (  ( (*limites)[0].second - (*limites)[0].first )/ l) * d;
        fx = cos(20 * x) - (abs(x) / 2) + (pow(x, 3) / 4);
        max = (fx + 4) / 6;
        min = 1 - (fx + 4) / 6;
        (*fitness)[i] = (max);
    }

    return 0;
}

int Dominio_Binario::radiosSTLX(){
    int valor_st, valor_lx;
    double funcao_objetivo, penalizacao;

    for(size_t i = 0; i < tamanho_populacao; i++){
        valor_st = 0;
        valor_lx = 0;
        for(size_t j = 0; j < tamanho_cromossomo / 2; j++){
            valor_st += (*individuos)[i][j] * pow(2, j);
            valor_lx += (*individuos)[i][j + tamanho_cromossomo / 2] * pow(2, j);
        }
        valor_st = roundf(24.0 / 31 * valor_st);
        valor_lx = roundf(16.0 / 31 * valor_lx);
        penalizacao = max(0.0, ((valor_st + 2 * valor_lx - 40) * 1.0) / 16);
        funcao_objetivo = ((30 * valor_st + 40 * valor_lx) * 1.0) / 1360;
        (*fitness)[i] = (funcao_objetivo - penalizacao);
    }
    return 0;
    
}

int Dominio_Inteiro_Permutado::NQueens(){
    int colisoes;
    int indice_pior = -1;
    double pior = 1.1;

    for(int i = 0; i < tamanho_populacao; i++){ // percorre todas as soluções
        colisoes = 0;
        for(int j = 0; j < tamanho_cromossomo; j++){ // percorre todas as rainha de uma solução
            int y = (*individuos)[i][j];
            int x = j;
            int yc = y + 1;
            int yb = y - 1;
            for(int xx1 = x + 1, xx2 = x - 1; xx1 < tamanho_cromossomo or xx2 >= 0; xx1++, xx2--){ // compara todas as rainhas com uma das rainhas
                int q1, q2;

                if (xx1 < tamanho_cromossomo)
                {
                    q1 = (*individuos)[i][xx1];
                }else{
                    q1 = -1;
                }
                
                if(xx2 >= 0){
                    q2 = (*individuos)[i][xx2];
                }else{
                    q2 = -1;
                }

                if(yc < tamanho_cromossomo){
                    if(yc == q1 or yc == q2){
                        colisoes++;
                        break;
                    }
                }
                if(yb >= 0){
                    if(yb == q1 or yb == q2){
                        colisoes++;
                        break;
                    }
                }
                yc++;
                yb--;
            }
        }
        (*fitness)[i] = (1 - (colisoes * 1.0) / tamanho_cromossomo);

        if((*fitness)[i] < pior){
            pior = (*fitness)[i];
            indice_pior = i;
        }
    }
    return indice_pior;
    
}