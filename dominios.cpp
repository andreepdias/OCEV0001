#include <bits/stdc++.h>
using namespace std;

#define BINARIO 0
#define INTEIRO 1
#define INTEIRO_PERMUTADO 2
#define REAL 3


class Dominio_Binario{
private:
    int tamanho_populacao, tamanho_cromossomo;
    vector<pair<double, double> > *limites;
    vector<vector<bool> > *individuos;
public:
    Dominio_Binario(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<bool> > *_i){
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        (*individuos).resize(tamanho_populacao, vector<bool>(tamanho_cromossomo));
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

    vector<double> funcaoCOS();
    vector<double> radiosSTLX();
};

class Dominio_Inteiro{
private:
    int tamanho_populacao, tamanho_cromossomo;
    vector<pair<double, double> > *limites;
    vector<vector<int> > *individuos;

public:
    Dominio_Inteiro(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<int> > *_i){
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        (*individuos).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
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
};

class Dominio_Inteiro_Permutado{
private:
    int tamanho_populacao, tamanho_cromossomo;
    vector<pair<double, double> > *limites;
    vector<vector<int>> *individuos;

  public:
    Dominio_Inteiro_Permutado(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<int>> *_i){
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        (*individuos).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        random_device device;
        mt19937 engine(device());

        for (int i = 0; i < tamanho_populacao; i++){
            for (int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = j;
            }
            shuffle((*individuos)[i].begin(), (*individuos)[i].end(), engine);
        }
    }

    vector<double> NQueens();
};

class Dominio_Real{
private:
    int tamanho_populacao, tamanho_cromossomo;
    vector<pair<double, double> > *limites;
    vector<vector<double> > *individuos;
public:
    Dominio_Real(int _tp, int _tc, vector<pair<double, double> > *_l, vector<vector<double> > *_i){
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        individuos = _i;
        (*individuos).resize(tamanho_populacao, vector<double>(tamanho_cromossomo));
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
};

