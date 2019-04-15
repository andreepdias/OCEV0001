#ifndef __DOMINIOS_CPP
#define __DOMINIOS_CPP

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <functional>
#include <map>
#include <cfloat>


using namespace std;

#define BINARIO 0
#define INTEIRO 1
#define INTEIRO_PERMUTADO 2
#define REAL 3


class Dominio_Binario{
private:
    int tamanho_populacao, tamanho_cromossomo;
    int probabilidade_crossover, probabilidade_mutacao;
    vector<pair<double, double> > *limites;
    vector<vector<bool> > *individuos;
    vector<vector<bool> > *individuos_intermediarios;
    vector<bool> *melhor_individuo;
    vector<double> *fitness;
    vector<int> *individuos_selecionados;

    random_device device{};
    mt19937 engine{device()};
public:
    Dominio_Binario(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<bool> > *_i, vector<vector<bool> > *_ii, vector<bool> *_m, vector<double> *_f, vector<int> *_is, int _pc, int _pm){
        fitness = _f;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        melhor_individuo = _m;
        individuos_intermediarios = _ii;
        probabilidade_crossover = _pc;
        probabilidade_mutacao = _pm;
        (*melhor_individuo).resize(tamanho_cromossomo);
        (*individuos).resize(tamanho_populacao, vector<bool>(tamanho_cromossomo));
        (*individuos_intermediarios).resize(tamanho_populacao, vector<bool>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        mt19937 engine(random_device{}());
        bernoulli_distribution distribution;

        for (int i = 0; i < tamanho_populacao; i++){
            for (int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = distribution(engine);
            }
        }
    }

    void funcaoCOS();
    void radiosSTLX();

    void crossover_n_cortes_bin(int qtd_pontos_corte);
    void crossover_uniforme_bin();
};

class Dominio_Inteiro{
private:
    int tamanho_populacao, tamanho_cromossomo;
    int probabilidade_crossover, probabilidade_mutacao;
    vector<pair<double, double> > *limites;
    vector<vector<int> > *individuos;
    vector<vector<int>> *individuos_intermediarios;
    vector<int> *melhor_individuo;
    vector<double> *fitness;
    vector<int> *individuos_selecionados;

    random_device device{};
    mt19937 engine{device()};

public:
    Dominio_Inteiro(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<int> > *_i, vector<vector<int> > *_ii, vector<int> *_m, vector<double> *_f, vector<int> *_is, int _pc, int _pm){
        fitness = _f;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        melhor_individuo = _m;
        individuos_intermediarios = _ii;
        probabilidade_crossover = _pc;
        probabilidade_mutacao = _pm;
        (*melhor_individuo).resize(tamanho_cromossomo);
        (*individuos).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
        (*individuos_intermediarios).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        random_device device;
        mt19937 engine(device());
        uniform_int_distribution<int> distribution((*limites)[0].first, (*limites)[0].second);

        for(int i = 0; i < tamanho_populacao; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = distribution(engine);
            }
        }
    }

    void crossover_n_cortes_int(int qtd_pontos_corte);
    void crossover_uniforme_int();
};

class Dominio_Inteiro_Permutado{
private:
    int tamanho_populacao, tamanho_cromossomo;
    int probabilidade_crossover, probabilidade_mutacao;
    vector<pair<double, double>> *limites;
    vector<vector<int> > *individuos;
    vector<vector<int>> *individuos_intermediarios;
    vector<int> *melhor_individuo;
    vector<double> *fitness;
    vector<int> *individuos_selecionados;

    random_device device{};
    mt19937 engine{device()};

  public:
    Dominio_Inteiro_Permutado(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<int>> *_i, vector<vector<int>> *_ii, vector<int> *_m, vector<double> *_f, vector<int> *_is, int _pc, int _pm){
        fitness = _f;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        melhor_individuo = _m;
        individuos_intermediarios = _ii;
        probabilidade_crossover = _pc;
        probabilidade_mutacao = _pm;
        (*melhor_individuo).resize(tamanho_cromossomo);
        (*individuos).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
        (*individuos_intermediarios).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        random_device device;
        mt19937 engine(device());

        for (int i = 0; i < tamanho_populacao; i++){
            for (int j = 0; j < tamanho_cromossomo; j++)
            {
                (*individuos)[i][j] = j;
            }
            shuffle((*individuos)[i].begin(), (*individuos)[i].end(), engine);
        }
    }

    void NQueens();

    void crossover_pmx_intp();

    void swap_mutation();
};

class Dominio_Real{
private:
    int tamanho_populacao, tamanho_cromossomo;
    int probabilidade_crossover, probabilidade_mutacao;
    vector<pair<double, double> > *limites;
    vector<vector<double> > *individuos;
    vector<vector<double>> *individuos_intermediarios;
    vector<double> *melhor_individuo;
    vector<double> *fitness;
    vector<int> *individuos_selecionados;

    random_device device{};
    mt19937 engine{device()};
public:
    Dominio_Real(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<double> > *_i, vector<vector<double> > *_ii, vector<double> *_m, vector<double> *_f, vector<int> *_is, int _pc, int _pm){
        fitness = _f;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        melhor_individuo = _m;
        individuos_intermediarios = _ii;
        probabilidade_crossover = _pc;
        probabilidade_mutacao = _pm;
        (*melhor_individuo).resize(tamanho_cromossomo);
        (*individuos).resize(tamanho_populacao, vector<double>(tamanho_cromossomo));
        (*individuos_intermediarios).resize(tamanho_populacao, vector<double>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        random_device device{};
        mt19937 engine{device()};
        uniform_real_distribution<double> distribution{(*limites)[0].first, (*limites)[0].second};

        for(int i = 0; i < tamanho_populacao; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = distribution(engine);
            }
        }
    }

    void crossover_blx_real(double alpha = 0.5);
    void crossover_media_uniforme_real();
    void crossover_aritmetico_real(double alpha = 0.5);
};


#endif