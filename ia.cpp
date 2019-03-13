#include <bits/stdc++.h>
using namespace std;

#define BIN 0
#define INT 1
#define INT_PERM 2
#define REAL 3

template <class T>
class Operadores{

    <T> operador;

    Operadores(int tipo){
        switch(tipo){
            x = new Operadores_Inteiros();
        }
    }
    oper
};

class Operadores_Inteiros{
public:
    int locus_aleatorio(){
        return rand();
    }
};
class Operadores_Binarios{
public:
    bool locus_aleatorio(){
        return rand() % 2 == 0 ? true : false;
    }
};
class Operadores_Reais{
public:
    double locus_aleatorio(){
        return rand();
    }
};


// dominio
// limites upperbound, lowerbound (reais, inteiros)
// integridade (intervalo)
template <class T>
class Populacao{
public:
    int tamanho_pop;
    int tamanho_cromossomo; //dimensionalidade

    vector<vector<T> > individuos;

    Populacao(int tp, int tc){
        tamanho_pop = tp;
        tamanho_cromossomo = tc;

        individuos.resize(tamanho_pop, vector<T>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(Operadores_Inteiros op){
        for(int i = 0; i < tamanho_pop; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                individuos[i][j] = op.locus_aleatorio();
            }
        }
    }
    void gerar_populacao_inicial(Operadores_Binarios op){
        for(int i = 0; i < tamanho_pop; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                individuos[i][j] = op.locus_aleatorio();
            }
        }
    }
    void gerar_populacao_inicial(Operadores_Reais op){
        for(int i = 0; i < tamanho_pop; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                individuos[i][j] = op.locus_aleatorio();
            }
        }
    }
    void print_individuos(){
        for(int i = 0; i < tamanho_pop; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                cout << individuos[i][j] << "\t";
            }
            cout << endl;
        }
    }
};


int main(){
    srand(time(NULL));

    int cod, pop, d, tipo;

    ifstream arquivo_parametros;
    arquivo_parametros.open("parametros.txt");

    arquivo_parametros >> cod;
    arquivo_parametros >> pop;
    arquivo_parametros >> d;

    Operadores_Binarios op;

    Populacao <bool> individuos(pop, d);
    individuos.gerar_populacao_inicial(op);
    individuos.print_individuos();


}
