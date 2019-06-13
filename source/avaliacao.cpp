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

    void *dados_problema;

    Avaliacao(){}    
    Avaliacao(Parametros *_p, Populacao *_populacao){
        p = _p;
        populacao = _populacao;

        carregar_dados_problema();
    }

    void avaliacao(){
        switch((*p).problema){
            case RAINHAS:       NQueens(); break;
            case RAINHAS_LUCRO: break;
            case RADIOS: break;
            case LABIRINTO:     labirinto(); break;
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

            }else if(fitness < segundo_pior_individuo.second){
                segundo_pior_individuo.first = i;
                segundo_pior_individuo.second = fitness;
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

    void labirinto(){

        vector<vector<int> > *matrix_labirinto = (vector<vector<int> >*)dados_problema;


        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        int sx = (*matrix_labirinto).size();
        int sy = (*matrix_labirinto)[0].size();


        for(int i = 0;  i< (*p).tamanho_populacao; i++){ 
            int celulas_validas = 0, celulas_invalidas = 0, celulas_diferentes = 0;
            int x = 10, y = 1;
            bool celula_final = false;
            int numero_movimentos = (*p).tamanho_cromossomo;

            vector<vector<bool> > visitados(sx, vector<bool> (sy, false));
            visitados[x][y] = true;

            double fitness_movimentos = 1.0;
            pair<int, int> anterior = make_pair(10, 1);

            vector<int> *c = (vector<int>*)(*populacao).individuos[i].cromossomo;

            for(int j = 0; j < (*p).tamanho_cromossomo; j++){
                int k = (*c)[j];

                int c = 0, aux_k = 0;
                while((*matrix_labirinto)[x + dx[k]][y + dy[k]] == 0){
                    k = (k + 1) % 4;
                    if ((*matrix_labirinto)[x + dx[k]][y + dy[k]] != 0){
                        if (!visitados[x + dx[k]][y + dy[k]]){
                            break;                        
                        }else{
                            aux_k = k;
                        }
                    }
                    c++;
                    if(c == 4){
                        k = aux_k;
                        break;
                    }
                }

                if((*matrix_labirinto)[x + dx[k]][y + dy[k]] != 0 and !visitados[x + dx[k]][y + dy[k]]){
                    celulas_validas++;

                    x = x + dx[k];
                    y = y + dy[k];

                    if(!visitados[x][y]){
                        visitados[x][y] = true;
                        celulas_diferentes++;
                    }
                }
                
                if((*matrix_labirinto)[x][y] == 3){
                    celula_final = true;
                    numero_movimentos = j + 1;
                    break;
                }
            }
            double distancia = 1 - x + 20  - y;
            celulas_invalidas = numero_movimentos - celulas_validas;

            (*populacao).individuos[i].funcao_objetivo = celulas_diferentes;
            (*populacao).individuos[i].infracao = celulas_invalidas;
            (*populacao).individuos[i].fitness = (celulas_diferentes / (numero_movimentos + pow(celulas_invalidas, 2)));
            //  * 0.6 + (1.0 - (distancia / 55.0)) * 0.4;
        }
    }

    void carregar_dados_problema(){
        switch((*p).problema){
            case LABIRINTO:
                dados_problema = (void*)new vector<vector<int> > (30, vector<int> (25));
                montar_labirinto();
                break;
        }
    }
    void montar_labirinto(){

        vector<vector<int> > *matrix_labirinto = (vector<vector<int> >*)dados_problema;

        (*matrix_labirinto) = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 3, 1, 1, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
            {0, 2, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0},
            {0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0},
            {0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
            {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
            {0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
            {0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
    }
};

#endif