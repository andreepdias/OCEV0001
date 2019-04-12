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

    vector<vector<bool> > *individuos_intermediarios_binario = NULL;
    vector<vector<int> > *individuos_intermediarios_inteiro = NULL;
    vector<vector<int> > *individuos_intermediarios_inteiro_permutado = NULL;
    vector<vector<double> > *individuos_intermediarios_real = NULL;

    vector<bool> *melhor_individuo_binario = NULL;
    vector<int> *melhor_individuo_inteiro = NULL;
    vector<int> *melhor_individuo_inteiro_permutado = NULL;
    vector<double> *melhor_individuo_real = NULL;

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
            individuos_intermediarios_binario = new vector<vector<bool> >();
            db = new Dominio_Binario(tamanho_populacao, tamanho_cromossomo, limites, individuos_binario, individuos_intermediarios_binario, melhor_individuo_binario, fitness);
            break;
        case INTEIRO:
            individuos_inteiro = new vector<vector<int> >();
            individuos_intermediarios_inteiro = new vector<vector<int> >();
            di = new Dominio_Inteiro(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro, individuos_intermediarios_inteiro, melhor_individuo_inteiro, fitness);
            break;
        case INTEIRO_PERMUTADO:
            individuos_inteiro_permutado = new vector<vector<int> >();
            individuos_intermediarios_inteiro_permutado = new vector<vector<int> >();
            dp = new Dominio_Inteiro_Permutado(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro_permutado, individuos_intermediarios_inteiro_permutado, melhor_individuo_inteiro_permutado, fitness);
            break;
        case REAL:
            individuos_real = new vector<vector<double> >();
            individuos_intermediarios_real = new vector<vector<double> >();
            dr = new Dominio_Real(tamanho_populacao, tamanho_cromossomo, limites, individuos_real, individuos_intermediarios_real, melhor_individuo_real, fitness);
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
        // print_populacao();
    }

    void Fitness(string problema){
        int pior = -1;
        if(problema == "NQueens"){
            pior = (*dp).NQueens();
        }else if(problema == "FuncaoCOS"){
            pior = (*db).funcaoCOS();
        }else if(problema == "RadiosSTLX"){
            pior = (*db).radiosSTLX();
        }
        // print_fitness();
        switch(tipo_variavel){
            case BINARIO:
                (*individuos_binario)[pior] = (*melhor_individuo_binario); break;
            case INTEIRO:
                (*individuos_inteiro)[pior] = (*melhor_individuo_inteiro); break;
            case INTEIRO_PERMUTADO:
                (*individuos_inteiro_permutado)[pior] = (*melhor_individuo_inteiro_permutado); break;
            case REAL:
                (*individuos_real)[pior] = (*melhor_individuo_real); break;
        }
    }

    void selecao(int tipo_selecao, void *parametros_selecao, bool print){
        switch(tipo_selecao){
            case 1: selecao_roleta(parametros_selecao); break;
            case 2: selecao_ranking(parametros_selecao); break;
            case 3: selecao_torneio(parametros_selecao); break;
            case 4: selecao_vizinhanca(parametros_selecao); break;
        }

        double pior = 1.0, melhor = 0.0, media = 0.0, k;
        int indice_melhor;

        for(int i = 0; i < tamanho_populacao; i++){
            k = (*fitness)[i];
            pior = min(k, pior);
            media += k;

            if(k > melhor){
                melhor = k;
                indice_melhor = i;
            }
            switch(tipo_variavel){
                case BINARIO:
                    (*individuos_intermediarios_binario)[i] = (*individuos_binario)[individuos_selecionados[i]]; break;
                case INTEIRO:
                    (*individuos_intermediarios_inteiro)[i] = (*individuos_inteiro)[individuos_selecionados[i]]; break;
                case INTEIRO_PERMUTADO:
                    (*individuos_intermediarios_inteiro_permutado)[i] = (*individuos_inteiro_permutado)[individuos_selecionados[i]]; break;
                case REAL:
                    (*individuos_intermediarios_real)[i] = (*individuos_real)[individuos_selecionados[i]]; break;
            }
        }
        media /= tamanho_populacao;
        switch(tipo_variavel){
            case BINARIO:
                (*individuos_binario) = (*individuos_intermediarios_binario); break;
                (*melhor_individuo_binario) = (*individuos_binario)[indice_melhor];
            case INTEIRO:
                (*individuos_inteiro) = (*individuos_intermediarios_inteiro); break;
                (*melhor_individuo_inteiro) = (*individuos_inteiro)[indice_melhor];
            case INTEIRO_PERMUTADO:
                (*individuos_inteiro_permutado) = (*individuos_intermediarios_inteiro_permutado); break;
                (*melhor_individuo_inteiro_permutado) = (*individuos_inteiro_permutado)[indice_melhor];
            case REAL:
                (*individuos_real) = (*individuos_intermediarios_real); break;
                (*melhor_individuo_real) = (*individuos_real)[indice_melhor];
        }
        if(print){
            printf("%lf %lf %lf\n", pior, media, melhor);
        }


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
    void print_cromossomo(int g){
        printf("%d.\t", g);
        for (int j = 0; j < tamanho_cromossomo; j++){
            switch(tipo_variavel){
                case BINARIO:
                    cout << (*individuos_binario)[g][j]; break;
                case INTEIRO:
                    cout << (*individuos_inteiro)[g][j]; break;
                case INTEIRO_PERMUTADO:
                    cout << (*individuos_inteiro_permutado)[g][j]; break;
                case REAL:
                    cout << (*individuos_real)[g][j]; break;
            }
            printf(" ");
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
    void crossover_n_cortes_bin(int qtd_pontos_corte);
    void crossover_uniforme_bin();

    void crossover_n_cortes_int(int qtd_pontos_corte);
    void crossover_uniforme_int();
    void crossover_blx_real(double alpha = 0.5);
    void crossover_media_uniforme_real();
    void crossover_aritmetico_real(double alpha = 0.5);

    void crossover_pmx_intp();

    void swap_mutation(double probabilidade);
    void bit_flip(double probabilidade);
    void mutacao_delta(double limite_inferior, double limite_superior, int denominador);

    };
