#include <bits/stdc++.h>
#include "dominios.cpp"
using namespace std;


class Populacao{
private:
    int tipo_variavel;
    int tamanho_populacao;
    int tamanho_cromossomo;

    vector<pair<double, double> > *limites;

    vector<vector<bool> > *individuos_binario = NULL;
    vector<vector<int> > *individuos_inteiro = NULL;
    vector<vector<int> > *individuos_inteiro_permutado = NULL;
    vector<vector<double> > *individuos_real = NULL;

    Dominio_Binario *db = NULL;
    Dominio_Inteiro_Permutado *dp = NULL;
    Dominio_Inteiro *di = NULL;
    Dominio_Real *dr = NULL;

    vector<double> *fitness;
    vector<int> individuos_selecionados;

    random_device device{};
    mt19937 engine{device()};

public:
    Populacao(int _tv, int _tp, int _tc, vector<pair<double, double> > *_l){
        tipo_variavel = _tv;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        fitness = new vector<double>();
        (*fitness).resize(tamanho_populacao);
        individuos_selecionados.resize(tamanho_populacao);
        switch (tipo_variavel){
        case BINARIO:
            individuos_binario = new vector<vector<bool> >();
            db = new Dominio_Binario(tamanho_populacao, tamanho_cromossomo, limites, individuos_binario, fitness);
            break;
        case INTEIRO:
            individuos_inteiro = new vector<vector<int> >();
            di = new Dominio_Inteiro(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro, fitness);
            break;
        case INTEIRO_PERMUTADO:
            individuos_inteiro_permutado = new vector<vector<int> >();
            dp = new Dominio_Inteiro_Permutado(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro_permutado, fitness);
            break;
        case REAL:
            individuos_real = new vector<vector<double> >();
            dr = new Dominio_Real(tamanho_populacao, tamanho_cromossomo, limites, individuos_real, fitness);
            break;
        }
    }

    void gerar_populacao_inicial(){
        switch(tipo_variavel){
            case BINARIO:
                (*db).gerar_populacao_inicial();
                break;
            case INTEIRO:
                (*di).gerar_populacao_inicial();
                break;
            case INTEIRO_PERMUTADO:
                (*dp).gerar_populacao_inicial();
                break;
            case REAL:
                (*dr).gerar_populacao_inicial();
                break;
        }
        print_populacao();
    }

    void Fitness(string problema){
        if(problema == "NQueens"){
            (*dp).NQueens();
        }else if(problema == "FuncaoCOS"){
            (*db).funcaoCOS();
        }else if(problema == "RadiosSTLX"){
            (*db).radiosSTLX();
        }
        print_fitness();
    }

    void selecao(int tipo_selecao, void *parametros_selecao){
        switch(tipo_selecao){
            case 1: selecao_roleta(parametros_selecao); break;
            case 2: selecao_ranking(parametros_selecao); break;
            case 3: selecao_torneio(parametros_selecao); break;
            case 4: selecao_vizinhanca(parametros_selecao); break;
        }
        print_selecionados();
    }


    void print_populacao(){
        printf("Populacao:\n");
        for (int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t", i);
            for (int j = 0; j < tamanho_cromossomo; j++){
                switch(tipo_variavel){
                    case BINARIO:
                        cout << (*individuos_binario)[i][j]; break;
                    case INTEIRO:
                        cout << (*individuos_inteiro)[i][j]; break;
                    case INTEIRO_PERMUTADO:
                        cout << (*individuos_inteiro_permutado)[i][j]; break;
                    case REAL:
                        cout << (*individuos_real)[i][j]; break;
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("\n");
    }

    void print_fitness(){
        printf("Fitness:\n");
        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t%.4lf\n", i, (*fitness)[i]);
        }
        printf("\n");
    }
    void print_selecionados(){
        printf("Selecao:\n");
        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t%d\t(%.10lf)\n", i, individuos_selecionados[i], (*fitness)[individuos_selecionados[i]]);
        }
        printf("\n");
    }
    int girar_roleta(vector<pair<double, int> > &participantes, int ignorado);
    void roleta(vector<int> &individuos, function<double(int)> valor, int n_escolhidos, int &k);
    void selecao_roleta(void *parametros);
    void selecao_ranking(void *parametros);
    void selecao_torneio(void *parametros);
    void selecao_vizinhanca(void *parametros);

};