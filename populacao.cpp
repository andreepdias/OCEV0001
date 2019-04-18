#ifndef __POPULACAO_CPP
#define __POPULACAO_CPP

/*
elitismo
parametros
nradios
queens
grafico
*/

#include "dominios.cpp"
#include "parametros.cpp"
using namespace std;


class Populacao{
private:
    int tipo_variavel;
    int tipo_selecao;
    int tipo_crossover;
    int tipo_mutacao;
    int tamanho_populacao;
    int tamanho_cromossomo;
    double probabilidade_crossover;
    double probabilidade_mutacao;
    bool elitismo;
    int problema;


    int k; double kp;
    int t, d;
    
    int c;

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
    
    vector<double> *infracoes;
    vector<double> *funcoes_objetivo;

    double melhor_individuo_fitness;

    Dominio_Binario *db = NULL;
    Dominio_Inteiro_Permutado *dp = NULL;
    Dominio_Inteiro *di = NULL;
    Dominio_Real *dr = NULL;

    vector<double> *fitness;
    vector<int> *individuos_selecionados;

    random_device device{};
    mt19937 engine{device()};

public:
    Populacao(Parametros p){
        tipo_variavel = p.COD;
        tipo_selecao = p.COD_SELECTION;
        tipo_crossover = p.COD_CROSSOVER;
        tipo_mutacao = p.COD_MUTATION;

        tamanho_populacao = p.POP;
        tamanho_cromossomo = p.DIM;

        probabilidade_crossover = p.PC;
        probabilidade_mutacao = p.PM;

        elitismo = p.ELIT;

        problema = p.PROBLEM;

        k = p.kk;
        kp = p.kp;
        t = p.t;
        d = p.d;
        c = p.c;

        printf("PC: %lf\n", probabilidade_crossover);

        fitness = new vector<double>();
        (*fitness).resize(tamanho_populacao);
        
        individuos_selecionados = new vector<int>();
        (*individuos_selecionados).resize(tamanho_populacao);

        infracoes = new vector<double>();
        (*infracoes).resize(tamanho_populacao);

        funcoes_objetivo = new vector<double>();
        (*funcoes_objetivo).resize(tamanho_populacao);

        switch (tipo_variavel){
        case BINARIO:
            melhor_individuo_binario = new vector<bool>();
            individuos_binario = new vector<vector<bool> >();
            individuos_intermediarios_binario = new vector<vector<bool> >();
            db = new Dominio_Binario(tamanho_populacao, tamanho_cromossomo, limites, individuos_binario, individuos_intermediarios_binario, melhor_individuo_binario, fitness, individuos_selecionados, probabilidade_crossover, probabilidade_mutacao, infracoes, funcoes_objetivo);
            break;
        case INTEIRO:
            melhor_individuo_inteiro = new vector<int>();
            individuos_inteiro = new vector<vector<int> >();
            individuos_intermediarios_inteiro = new vector<vector<int> >();
            di = new Dominio_Inteiro(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro, individuos_intermediarios_inteiro, melhor_individuo_inteiro, fitness, individuos_selecionados, probabilidade_crossover, probabilidade_mutacao);
            break;
        case INTEIRO_PERMUTADO:
            melhor_individuo_inteiro_permutado = new vector<int>();
            individuos_inteiro_permutado = new vector<vector<int> >();
            individuos_intermediarios_inteiro_permutado = new vector<vector<int> >();
            dp = new Dominio_Inteiro_Permutado(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro_permutado, individuos_intermediarios_inteiro_permutado, melhor_individuo_inteiro_permutado, fitness, individuos_selecionados, probabilidade_crossover, probabilidade_mutacao);
            break;
        case REAL:
            melhor_individuo_real = new vector<double>();
            individuos_real = new vector<vector<double> >();
            individuos_intermediarios_real = new vector<vector<double> >();
            dr = new Dominio_Real(tamanho_populacao, tamanho_cromossomo, limites, individuos_real, individuos_intermediarios_real, melhor_individuo_real, fitness, individuos_selecionados, probabilidade_crossover, probabilidade_mutacao);
            break;
        }
    }

    void gerar_populacao_inicial(){
        switch(tipo_variavel){
            case BINARIO:
                (*db).gerar_populacao_inicial();
                (*melhor_individuo_binario) = (*individuos_binario)[0];
                melhor_individuo_fitness = 0;
                break;
            case INTEIRO:
                (*di).gerar_populacao_inicial();
                (*melhor_individuo_inteiro) = (*individuos_inteiro)[0];
                melhor_individuo_fitness = 0;
                break;
            case INTEIRO_PERMUTADO:
                (*dp).gerar_populacao_inicial();
                (*melhor_individuo_inteiro_permutado) = (*individuos_inteiro_permutado)[0];
                melhor_individuo_fitness = 0;
                break;
            case REAL:
                (*dr).gerar_populacao_inicial();
                (*melhor_individuo_real) = (*individuos_real)[0];
                melhor_individuo_fitness = 0;
                break;
        }
    }

    void Fitness(int k, ofstream &out){
        if(problema == 1){
            (*dp).NQueens();
        }else if(problema == 2){
            (*db).radiosSTLX();
        }
        /*
        else if(problema == "FuncaoCOS"){
            (*db).funcaoCOS();
        }else if(problema == "RadiosSTLX"){
            (*db).radiosSTLX();
        }
        */

        double pior, melhor, media;
        int indice_melhor, indice_pior;
        pior = 1.1, melhor = -0.1, media = 0.0;
        for(int i = 0; i < tamanho_populacao; i++){

            double fit = (*fitness)[i];
            media += fit;

            if(fit >= melhor){
                indice_melhor = i;
                melhor = fit;
            }
            if(fit <= pior){
                indice_pior = i;
                pior = fit;
            }
        }
        media /= tamanho_populacao;

        if(elitismo){
            /* TROCA PIOR ÍNDIVIDUO PELO MELHOR DA GERAÇÃO ANTERIOR */        
            switch(tipo_variavel){
                case BINARIO:
                    (*fitness)[indice_pior] = melhor_individuo_fitness;
                    (*individuos_binario)[indice_pior] = (*melhor_individuo_binario); break;
                case INTEIRO:
                    (*fitness)[indice_pior] = melhor_individuo_fitness;
                    (*individuos_inteiro)[indice_pior] = (*melhor_individuo_inteiro); break;
                case INTEIRO_PERMUTADO:
                    (*fitness)[indice_pior] = melhor_individuo_fitness;
                    (*individuos_inteiro_permutado)[indice_pior] = (*melhor_individuo_inteiro_permutado); break;
                case REAL:
                    (*fitness)[indice_pior] = melhor_individuo_fitness;
                    (*individuos_real)[indice_pior] = (*melhor_individuo_real); break;
            }
            /* MEMORIZA MELHOR INDIVIDUO DESSA GERAÇÃO */
            if(melhor >= melhor_individuo_fitness){
                melhor_individuo_fitness = melhor;
                switch(tipo_variavel){
                    case BINARIO:
                        (*melhor_individuo_binario) = (*individuos_binario)[indice_melhor];
                        break;
                    case INTEIRO:
                        (*melhor_individuo_inteiro) = (*individuos_inteiro)[indice_melhor];
                        break;
                    case INTEIRO_PERMUTADO:
                        (*melhor_individuo_inteiro_permutado) = (*individuos_inteiro_permutado)[indice_melhor];
                        break;
                    case REAL:
                        (*melhor_individuo_real) = (*individuos_real)[indice_melhor];
                        break;
                }
            }
            melhor = max(melhor, melhor_individuo_fitness);
        }
        printf("%d %lf %lf %lf\n", k, melhor, pior, media);
        out << k << " " << melhor << " " << pior << " " << media << endl;
        
    }


    void selecao(){
        switch(tipo_selecao){
            case 1: selecao_roleta(); break;
            case 2: selecao_ranking(); break;
            case 3: selecao_torneio(); break;
            case 4: selecao_vizinhanca(); break;
        }

        /* CRIA POPULAÇÃO INTERMEDIÁRIA */
        for(int i = 0; i < tamanho_populacao; i++){
            int s = (*individuos_selecionados)[i];
            switch(tipo_variavel){
                case BINARIO:
                    (*individuos_intermediarios_binario)[i] = (*individuos_binario)[s]; break;
                case INTEIRO:
                    (*individuos_intermediarios_inteiro)[i] = (*individuos_inteiro)[s]; break;
                case INTEIRO_PERMUTADO:
                    (*individuos_intermediarios_inteiro_permutado)[i] = (*individuos_inteiro_permutado)[s]; break;
                case REAL:
                    (*individuos_intermediarios_real)[i] = (*individuos_real)[s]; break;
            }
        }

        /* ATUALIZA POPULAÇÃO PRINCIPAL */
        switch(tipo_variavel){
            case BINARIO:
                (*individuos_binario) = (*individuos_intermediarios_binario); break;
            case INTEIRO:
                (*individuos_inteiro) = (*individuos_intermediarios_inteiro); break;
            case INTEIRO_PERMUTADO:
                (*individuos_inteiro_permutado) = (*individuos_intermediarios_inteiro_permutado); break;
            case REAL:
                (*individuos_real) = (*individuos_intermediarios_real); break;
        }
    }

    void crossover(){
        switch(tipo_crossover){
        /*
            case 1:
                (*db).crossover_n_cortes_bin(c); break;
            case 2:
                (*db).crossover_n_cortes_int(c); break;
            case 3:
                (*di).crossover_uniforme_bin(c); break;
            case 4:
                (*di).crossover_uniforme_int(); break;
            case 5:
                (*dr).crossover_aritmetico_real(); break;
            case 6:
                (*dr).crossover_media_uniforme_real(); break;
                */
        case 8  :
            (*dp).crossover_pmx_intp();
        }
    }

    void mutation(){
        switch(tipo_mutacao){
            case 1:
                bit_flip(); break;
            case 3:
                (*dp).swap_mutation(); break;
        }
    }

    void print_finalizacao_execucao(){
        double melhor = -0.1;
        int indice_melhor;
        for(int i = 0; i < tamanho_populacao; i++){
            if((*fitness)[i] > melhor){
                melhor = (*fitness)[i];
                indice_melhor = i;
            }
        }
        vector<pair<string, double> > variaveis = (*db).calcula_variaveis_radios(indice_melhor);
        printf("Melhor individuo:\n\tFitness: %lf\n\tFuncao Objetivo: %lf\n\tPenalizacao: %lf\n", (*fitness)[indice_melhor], (*funcoes_objetivo)[indice_melhor], (*infracoes)[indice_melhor]);
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
            printf("\t(%lf)", (*fitness)[i]);
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
            printf("%d.\t%d\t(%.10lf)\n", i, (*individuos_selecionados)[i], (*fitness)[(*individuos_selecionados)[i]]);
        }
        printf("\n");
    }


    int girar_roleta(vector<pair<double, int> > &participantes, int ignorado);
    void roleta(vector<int> &individuos, function<double(int)> valor, int n_escolhidos, int &k);

    void selecao_roleta();
    void selecao_ranking();
    void selecao_torneio();
    void selecao_vizinhanca();

    void bit_flip();

    // void crossover_n_cortes_bin(int qtd_pontos_corte);
    // void crossover_uniforme_bin();
    

};

#endif