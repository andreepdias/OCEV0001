#ifndef __INDIVIDUOS_CPP
#define __INDIVIDUOS_CPP
using namespace std;

#include "parametros.cpp"

class Individuo{
private:
    Parametros *p;

public:
    double fitness;
    double funcao_objetivo;
    double infracao;

    void *cromossomo = NULL;
    void *dados_individuo = NULL;

    Individuo(){}
    Individuo(Parametros *_p){
        p = _p;

        fitness = 0;
        funcao_objetivo = 0;
        infracao = 0;

        criar_cromossomo();
        criar_dados_individuo();
        
    }

    void criar_cromossomo(){
        switch((*p).tipo_variavel){
            case BINARIO:
                cromossomo = (void*)new vector<bool>((*p).tamanho_cromossomo);
                break;
            case INTEIRO:
                cromossomo = (void*)new vector<int>((*p).tamanho_cromossomo);
                break;
            case INTEIRO_PERMUTADO:
                cromossomo = (void*)new vector<int>((*p).tamanho_cromossomo);
                break;
            case REAL:
                cromossomo = (void*)new vector<double>((*p).tamanho_cromossomo);
                break;
        }
        vector<int> *c1 = (vector<int>*)cromossomo;
    }

    void gerar_cromossomo_aleatorio(mt19937 &engine){
        switch((*p).tipo_variavel){
            case BINARIO:           
                gerar_cromossomo_aleatorio_binario(engine); break;
            case INTEIRO:           
                gerar_cromossomo_aleatorio_inteiro(engine); break;
            case INTEIRO_PERMUTADO: 
                gerar_cromossomo_aleatorio_inteiro_permutado(engine); break;
            case REAL:              
                gerar_cromossomo_aleatorio_real(engine); break;
        }
    }
    void gerar_cromossomo_aleatorio_binario(mt19937 &engine){
        bernoulli_distribution distribution;

        vector<bool> *c = (vector<bool>*)cromossomo;

        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            (*c)[i] = distribution(engine);
        }
    }
    void gerar_cromossomo_aleatorio_inteiro(mt19937 &engine){
        uniform_int_distribution<int> distribution((*p).limite_inferior, (*p).limite_superior);

        vector<int> *c = (vector<int>*)cromossomo;

        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            (*c)[i] = distribution(engine);
        }
    }
    void gerar_cromossomo_aleatorio_inteiro_permutado(mt19937 &engine){
        vector<int> *c = (vector<int>*)cromossomo;

        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            (*c)[i] = i;
        }
        shuffle((*c).begin(), (*c).end(), engine);
    }
    void gerar_cromossomo_aleatorio_real(mt19937 &engine){
        uniform_real_distribution<double> distribution((*p).limite_inferior, (*p).limite_superior);

        vector<double> *c = (vector<double>*)cromossomo;

        for(int i = 0; i < (*p).tamanho_cromossomo; i++){

            switch((*p).problema){
                case KEANES:
                    (*c)[i] = 5.0;
                    break;
                default:
                    (*c)[i] = distribution(engine);
                    break;
            }

        }
    }

    void print_cromossomo(){
        switch((*p).tipo_variavel){
            case BINARIO:           
                print_cromossomo_binario(); break;
            case INTEIRO:           
            case INTEIRO_PERMUTADO: 
                print_cromossomo_inteiro(); break;
            case REAL:              
                print_cromossomo_real(); break;
        }
        printf(" (%.5lf) (%.0lf) (%.0lf)\n", fitness, funcao_objetivo, infracao);
    }
    void print_cromossomo_binario(){
        vector<bool> *c = (vector<bool>*)cromossomo;
        
        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            cout << (*c)[i];
        }
    }
    void print_cromossomo_inteiro(){
        vector<int> *c = (vector<int>*)cromossomo;
        
        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            printf("%d ", (*c)[i]);
        }
    }
    void print_cromossomo_real(){
        vector<double> *c = (vector<double>*)cromossomo;
        
        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            printf("%.5lf ", (*c)[i]);
        }
    }

    void operator = (const Individuo &i){

        if(cromossomo == NULL){
            p = i.p;
            criar_cromossomo();
            criar_dados_individuo();
        }

        fitness = i.fitness;
        funcao_objetivo = i.funcao_objetivo;
        infracao = i.infracao;

        switch((*p).tipo_variavel){
            case BINARIO: 
                copy_cromossomo_binario(i); 
                break;
            case INTEIRO: 
            case INTEIRO_PERMUTADO:
                copy_cromossomo_inteiro(i); 
                break;
            case REAL: 
                copy_cromossomo_real(i); 
                break;
        }

        switch((*p).problema){
            case LABIRINTO:
                copy_dados_individuo_labirinto(i);
                break;
            case KEANES:
                copy_dados_individuo_keanes(i);
                break;
        }
    }

    void copy_cromossomo_binario(const Individuo &i){
        vector<bool> *c1 = (vector<bool>*)cromossomo;
        vector<bool> *c2 = (vector<bool>*)i.cromossomo;
        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            (*c1)[i] = (*c2)[i];
        }
    }
    void copy_cromossomo_inteiro(const Individuo &indiv){
        vector<int> *c1 = (vector<int>*)cromossomo;
        vector<int> *c2 = (vector<int>*)indiv.cromossomo;
        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            (*c1)[i] = (*c2)[i];
        }
    }
    void copy_cromossomo_real(const Individuo &i){
        vector<double> *c1 = (vector<double>*)cromossomo;
        vector<double> *c2 = (vector<double>*)i.cromossomo;
        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            (*c1)[i] = (*c2)[i];
        }
    }

    void criar_dados_individuo(){
        switch((*p).problema){
            case LABIRINTO:
                dados_individuo = (void*)new vector<vector<int> > (30, vector<int> (30, -1));
                break;
            case KEANES:
                dados_individuo = (void*)new vector<double> (5, 0);
                break;
        }
    }

    void copy_dados_individuo_labirinto(const Individuo &i){
        vector<vector<int> > *d1 = (vector<vector<int> > *)dados_individuo;
        vector<vector<int> > *d2 = (vector<vector<int> >*)i.dados_individuo;

        for(int i = 0; i < (*d2).size(); i++){
            for(int j = 0; j < (*d2)[0].size(); j++){
                (*d1)[i][j] = (*d2)[i][j];
            }
        }
    }

    void copy_dados_individuo_keanes(const Individuo &i){
        vector<double> *d1 = (vector<double>*)dados_individuo;
        vector<double> *d2 = (vector<double>*)i.dados_individuo;

        for(int i = 0; i < (*d2).size(); i++){
            (*d1)[i] = (*d2)[i];
        }
    }

    void print_restricoes(){
        switch((*p).problema){
            case KEANES:
                print_restricoes_keanes();
                break;
        }
    }

    void print_restricoes_keanes(){
        vector<double> *d = (vector<double>*)dados_individuo;

        printf("%.5lf\t%.5lf", (*d)[1], (*d)[2]);

    }
    

};

#endif