#ifndef __POPULACAO_CPP
#define __POPULACAO_CPP
using namespace std;

#include "individuo.cpp"

class Populacao{
private:
    Parametros *p;

public:
    vector<Individuo> individuos;
    Individuo melhor_individuo;

    Populacao(Parametros *_p){
        p = _p;

        for(int i = 0; i < (*p).tamanho_populacao; i++){
            individuos.push_back(Individuo(_p));
        }
        melhor_individuo = individuos[0];
    }
    ~Populacao(){
        for(int i = 0; i < (*p).tamanho_populacao; i++){
            free(individuos[i].cromossomo);
            free(individuos[i].dados_individuo);
        }
    }

    void  gerar_populacao_aleatoria(){
        mt19937 engine(random_device{}());

        for(int i = 0; i < (*p).tamanho_populacao; i++){
            individuos[i].gerar_cromossomo_aleatorio(engine);
        }
        melhor_individuo = individuos[0];
    }

    void print_populacao(){
        for(int i = 0; i < (*p).tamanho_populacao; i++){
            printf("%d. ", i);
            individuos[i].print_cromossomo();
        }
        printf("\n");
    }

    void operator = (const Populacao &pop){
        individuos = pop.individuos;
    }
};

#endif