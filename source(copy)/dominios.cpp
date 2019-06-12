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
#include <omp.h>

using namespace std;

#define BINARIO 0
#define INTEIRO 1
#define INTEIRO_PERMUTADO 2
#define REAL 3


class Dominio_Binario{
private:
    int tamanho_populacao, tamanho_cromossomo;
    double probabilidade_crossover, probabilidade_mutacao, *diversidade;
    vector<pair<double, double> > *limites;
    vector<vector<bool> > *individuos;
    vector<vector<bool> > *individuos_intermediarios;
    vector<bool> *melhor_individuo;
    vector<double> *fitness;
    vector<double> *infracoes;
    vector<double> *funcoes_objetivo;
    vector<int> *individuos_selecionados;

    random_device device{};
    mt19937 engine{device()};
public:
    Dominio_Binario(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<bool> > *_i, vector<vector<bool> > *_ii, vector<bool> *_m, vector<double> *_f, vector<int> *_is, double _pc, double _pm, vector<double> *_if, vector<double> *_fo, double *_diversidade){
        fitness = _f;
        infracoes = _if;
        funcoes_objetivo = _fo;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        melhor_individuo = _m;
        individuos_intermediarios = _ii;
        probabilidade_crossover = _pc;
        probabilidade_mutacao = _pm;
        diversidade = _diversidade;
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
    vector <pair<string, double> > calcula_variaveis_radios(int indice);

    void crossover_n_cortes_bin(int qtd_pontos_corte);
    void crossover_uniforme_bin();
};

class Dominio_Inteiro{
private:
    int tamanho_populacao, tamanho_cromossomo;
    int limite_inferior, limite_superior;
    double probabilidade_crossover, probabilidade_mutacao, *diversidade;
    vector<pair<double, double> > *limites;
    vector<vector<int> > *individuos;
    vector<vector<int>> *individuos_intermediarios;
    vector<int> *melhor_individuo;
    vector<double> *fitness;
    vector<double> *infracoes;
    vector<double> *funcoes_objetivo;
    vector<int> *individuos_selecionados;

    vector<vector<int> > matrix_labirinto;
    pair<int, int> coordenadas_melhor;

    random_device device{};
    mt19937 engine{device()};

public:
    Dominio_Inteiro(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<int> > *_i, vector<vector<int> > *_ii, vector<int> *_m, vector<double> *_f, vector<int> *_is, double _pc, double _pm, vector<double> *_if, vector<double> *_fo, double *_diversidade, int _li, int _ls){
        fitness = _f;
        infracoes = _if;
        funcoes_objetivo = _fo;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        limite_inferior = _li;
        limite_superior = _ls;
        individuos = _i;
        melhor_individuo = _m;
        individuos_intermediarios = _ii;
        probabilidade_crossover = _pc;
        probabilidade_mutacao = _pm;
        diversidade = _diversidade;
        (*melhor_individuo).resize(tamanho_cromossomo);
        (*individuos).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
        (*individuos_intermediarios).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));

        matrix_labirinto = {
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

    void gerar_populacao_inicial(){
        random_device device;
        mt19937 engine(device());
        uniform_int_distribution<int> distribution(limite_inferior, limite_superior);

        for(int i = 0; i < tamanho_populacao; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = distribution(engine);
            }
        }

    }
    void labirinto();
    double fx_infracao(int j);

    void crossover_n_cortes_int(int qtd_pontos_corte);
    void crossover_uniforme_int();

    void mutacao_inteira();
    vector<pair<string, double>> calcula_variaveis_labirinto(int indice);
};

class Dominio_Inteiro_Permutado{
private:
    int tamanho_populacao, tamanho_cromossomo, draw;
    double probabilidade_crossover, probabilidade_mutacao, *diversidade;
    vector<pair<double, double>> *limites;
    vector<vector<int> > *individuos;
    vector<vector<int>> *individuos_intermediarios;
    vector<int> *melhor_individuo;
    vector<double> *fitness;
    vector<double> *infracoes;
    vector<double> *funcoes_objetivo;
    vector<int> *individuos_selecionados;

    pair<double, vector<vector<double> > > *tabuleiro_lucro;
    vector<vector<bool> > *posicao_infracao;

    random_device device{};
    mt19937 engine{device()};

  public:
      Dominio_Inteiro_Permutado(int _tp, int _tc, vector<pair<double, double>> *_l, vector<vector<int>> *_i, vector<vector<int>> *_ii, vector<int> *_m, vector<double> *_f, vector<int> *_is, double _pc, double _pm, pair<double, vector<vector<double> > > *_tl, vector<double> *_if, vector<double> *_fo, vector<vector<bool> > *_pi, int _d, double *_diversidade)
      {
          fitness = _f;
          infracoes = _if;
          funcoes_objetivo = _fo;
          tamanho_populacao = _tp;
          tamanho_cromossomo = _tc;
          limites = _l;
          individuos = _i;
          melhor_individuo = _m;
          individuos_intermediarios = _ii;
          probabilidade_crossover = _pc;
          probabilidade_mutacao = _pm;
          tabuleiro_lucro = _tl;
          posicao_infracao = _pi;
          draw = _d;
          diversidade = _diversidade;
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
    void NQueensProfit();

    void crossover_pmx_intp();

    void swap_mutation();

    vector<pair<string, double> > calcula_variaveis_nqueens(int indice);

    void calcula_diversidade();
};

class Dominio_Real{
private:
    int tamanho_populacao, tamanho_cromossomo;
    double probabilidade_crossover, probabilidade_mutacao, *diversidade;
    vector<pair<double, double> > *limites;
    vector<vector<double> > *individuos;
    vector<vector<double>> *individuos_intermediarios;
    vector<double> *melhor_individuo;
    vector<double> *fitness;
    vector<double> *infracoes;
    vector<double> *funcoes_objetivo;
    vector<int> *individuos_selecionados;

    random_device device{};
    mt19937 engine{device()};
public:
    Dominio_Real(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<double> > *_i, vector<vector<double> > *_ii, vector<double> *_m, vector<double> *_f, vector<int> *_is, double _pc, double _pm, vector<double> *_if, vector<double> *_fo, double *_diversidade){
        fitness = _f;
        infracoes = _if;
        funcoes_objetivo = _fo;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        melhor_individuo = _m;
        individuos_intermediarios = _ii;
        probabilidade_crossover = _pc;
        probabilidade_mutacao = _pm;
        diversidade = _diversidade;
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