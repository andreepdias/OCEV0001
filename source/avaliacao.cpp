#ifndef __AVALIACAO_CPP
#define __AVALIACAO_CPP
using namespace std;

#include "populacao.cpp"

class Avaliacao {
public:
    Parametros *p;    
    Populacao *populacao;

    pair<int, double> melhor_individuo;
    pair<int, double> pior_individuo;
    pair<int, double> segundo_pior_individuo;

    double fitness_medio;

    Avaliacao(){}    
    Avaliacao(Parametros *_p, Populacao *_populacao){
        p = _p;
        populacao = _populacao;
    }

    void avaliacao(){
        switch((*p).problema){
            case RAINHAS:       NQueens(); break;
            case RAINHAS_LUCRO: break;
            case RADIOS: break;
            case LABIRINTO: break;
        }

        melhor_individuo.first = 0;
        melhor_individuo.second = (*populacao).individuos[0].fitness;

        pior_individuo.first = 0;
        pior_individuo.second = (*populacao).individuos[0].fitness;
        
        segundo_pior_individuo.first = 0;
        segundo_pior_individuo.second = (*populacao).individuos[0].fitness;

        fitness_medio = 0;

        for(int i = 1; i < (*p).tamanho_populacao; i++){
            double fitness = (*populacao).individuos[i].fitness;
            if(fitness > melhor_individuo.second){
                melhor_individuo.first = i;
                melhor_individuo.second = fitness;
            }
            if(fitness <= pior_individuo.second){

                segundo_pior_individuo.first = pior_individuo.first;
                segundo_pior_individuo.second = pior_individuo.second;

                pior_individuo.first = i;
                pior_individuo.second = fitness;
            }
            fitness_medio += fitness;
        }
        fitness_medio /= (*p).tamanho_populacao;
    }
    void NQueens()
    {
        #pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < (*p).tamanho_populacao; i++){ 
            vector<int> *c = (vector<int>*)(*populacao).individuos[i].cromossomo;
            int colisoes = 0;
            double fo = 0;
            for(int j = 0; j < (*p).tamanho_cromossomo; j++){
                int y = (*c)[j];
                int x = j;
                int yc = y + 1;
                int yb = y - 1;
                bool colidiu = false;
                for(int xx1 = x + 1, xx2 = x - 1; xx1 < (*p).tamanho_cromossomo or xx2 >= 0; xx1++, xx2--){ // compara todas as rainhas com uma das rainhas
                    int q1, q2;

                    if (xx1 < (*p).tamanho_cromossomo)
                    {
                        q1 = (*c)[xx1];
                    }else{
                        q1 = -1;
                    }
                    
                    if(xx2 >= 0){
                        q2 = (*c)[xx2];
                    }else{
                        q2 = -1;
                    }

                    if(yc < (*p).tamanho_cromossomo){
                        if(yc == q1 or yc == q2){
                            colisoes++;
                            colidiu = true;
                            break;
                        }
                    }
                    if(yb >= 0){
                        if(yb == q1 or yb == q2){
                            colisoes++;
                            colidiu = true;
                            break;
                        }
                    }
                    yc++;
                    yb--;
                }
            }
            fo = (*p).tamanho_cromossomo - colisoes;
            (*populacao).individuos[i].funcao_objetivo = fo;
            (*populacao).individuos[i].infracao = colisoes;
            (*populacao).individuos[i].fitness = fo / (*p).tamanho_cromossomo;
        }
    }
};

#endif