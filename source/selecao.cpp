#ifndef __SELECAO_CPP
#define __SELECAO_CPP
using namespace std;

#include "populacao.cpp"

class Selecao {
private:
    Parametros *p;
    
    Populacao *populacao;
    Populacao *populacao_intermediaria;

public:
    Selecao(){}    
    Selecao(Parametros *_p, Populacao *_populacao, Populacao *_populacao_intermediaria){
        p = _p;
        populacao = _populacao;
        populacao_intermediaria = _populacao_intermediaria;
    }

    void selecao(){
        switch((*p).tipo_selecao){
            case ROLETA: roleta(); break;
            case RANKING: break;
            case TORNEIO: torneio(); break;
            case VIZINHANCA: break;
        }
    }

    void torneio(){
        mt19937 engine(random_device{}());

        uniform_real_distribution<double> distribution_real{0.0, 1.0};
        uniform_int_distribution<int> distribution_int(0, (*p).tamanho_populacao - 1);
        uniform_int_distribution<int> distribution_int_k(1, (*p).torneio_k - 1);

        #pragma omp parallel
        #pragma omp for schedule(dynamic)

        for (int x = 0; x < (*p).tamanho_populacao; x++){

            vector<pair<double, int> > individuos_torneio((*p).torneio_k);
            map<int, bool> individuos_escolhidos;

            int r;
            double rr;

            individuos_escolhidos.clear();

            for (int i = 0; i < (*p).torneio_k; i++){
                do{
                    r = distribution_int(engine);
                }while(individuos_escolhidos.find(r) != individuos_escolhidos.end());

                individuos_escolhidos[r] = true;
                individuos_torneio[i].first = (*populacao).individuos[r].fitness;
                individuos_torneio[i].second = r;
            }
            sort(individuos_torneio.begin(), individuos_torneio.end());

            /* printf("Torneio com: ");
            for(auto &x : individuos_torneio){
                cout << x.second << " ";
            }
            cout << '\n'; */

            rr = distribution_real(engine);

            if ((*p).torneio_kp >= rr){
                int indice_individuo = individuos_torneio[(*p).torneio_k - 1].second;
                (*populacao_intermediaria).individuos[x] = (*populacao).individuos[indice_individuo];
                /* printf("Escolhido: %d\n", indice_individuo); */
            }else{
                r = distribution_int_k(engine);
                int indice_individuo = individuos_torneio[r].second;
                (*populacao_intermediaria).individuos[x] = (*populacao).individuos[indice_individuo];
            }
        }
    }
    
    void roleta(){
        mt19937 engine(random_device{}());

        uniform_real_distribution<double> distribution_real{0.0, 1.0};

        double somatorio_fitness = 0;
        for(int i = 0; i < (*p).tamanho_populacao; i++){
            somatorio_fitness += (*populacao).individuos[i].fitness;
        }

        #pragma omp parallel for schedule(dynamic)
        for(int x = 0; x < (*p).tamanho_populacao / 2; x++){

            vector<double> fitness_relativo((*p).tamanho_populacao);

            double somatorio_fitness_atual = somatorio_fitness;

            if(somatorio_fitness_atual == 0){
                for(int j = 0; j < (*p).tamanho_populacao; j++){
                    fitness_relativo[j] = 1.0 / (*p).tamanho_populacao;
                }
            }else{
                for(int j = 0; j < (*p).tamanho_populacao; j++){
                    fitness_relativo[j] = (*populacao).individuos[j].fitness / somatorio_fitness_atual;
                }
            }
            double soma_acumulada_roleta = 0;
            double random = distribution_real(engine);
            int individuo_escolhido_roleta = 0;
            for(int j = 0; j < (*p).tamanho_populacao; j++){
                double fitness = fitness_relativo[j];
                if(fitness + soma_acumulada_roleta >= random){
                    (*populacao_intermediaria).individuos[x * 2] = (*populacao).individuos[j];
                    individuo_escolhido_roleta = j;
                    break;
                }
                soma_acumulada_roleta += fitness;
            }

            somatorio_fitness_atual -= (*populacao).individuos[individuo_escolhido_roleta].fitness;

            if(somatorio_fitness_atual == 0){
                for(int j = 0; j < (*p).tamanho_populacao; j++){
                    fitness_relativo[j] = 1.0 / ((*p).tamanho_populacao - 1);
                }
            }else{
                for(int j = 0; j < (*p).tamanho_populacao; j++){
                    fitness_relativo[j] = (*populacao).individuos[j].fitness / somatorio_fitness_atual;
                }
            }
            soma_acumulada_roleta = 0;
            random = distribution_real(engine);
            for(int j = 0; j < (*p).tamanho_populacao; j++){
                if(individuo_escolhido_roleta == j) continue;
                double fitness = fitness_relativo[j];
                if(fitness + soma_acumulada_roleta >= random){
                    (*populacao_intermediaria).individuos[(x * 2) + 1] = (*populacao).individuos[j];
                    break;
                }
                soma_acumulada_roleta += fitness;
            }
        }
    }
    

};

#endif