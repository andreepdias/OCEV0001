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

public:
    Populacao(int _tv, int _tp, int _tc, vector<pair<double, double> > *_l){
        tipo_variavel = _tv;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limites = _l;
        switch (tipo_variavel){
        case BINARIO:
            individuos_binario = new vector<vector<bool> >();
            db = new Dominio_Binario(tamanho_populacao, tamanho_cromossomo, limites, individuos_binario);
            break;
        case INTEIRO:
            individuos_inteiro = new vector<vector<int> >();
            di = new Dominio_Inteiro(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro);
            break;
        case INTEIRO_PERMUTADO:
            individuos_inteiro_permutado = new vector<vector<int> >();
            dp = new Dominio_Inteiro_Permutado(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro_permutado);
            break;
        case REAL:
            individuos_real = new vector<vector<double> >();
            dr = new Dominio_Real(tamanho_populacao, tamanho_cromossomo, limites, individuos_real);
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
    }

    void fitness(string problema){
        vector<double> fitness;

        if(problema == "NQueens"){
            fitness = (*dp).NQueens();
        }else if(problema == "FuncaoCOS"){
            fitness = (*db).funcaoCOS();
        }else if(problema == "RadiosSTLX"){
            fitness = (*db).radiosSTLX();
        }

        print_fitness(fitness);
    }

    void print_populacao(){
        for (int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t", i);
            for (int j = 0; j < tamanho_cromossomo; j++){
                switch(tipo_variavel){
                    case BINARIO:
                        cout << (*individuos_binario)[i][j];
                        break;
                    case INTEIRO:
                        cout << (*individuos_inteiro)[i][j];
                        break;
                    case INTEIRO_PERMUTADO:
                        cout << (*individuos_inteiro_permutado)[i][j];
                        break;
                    case REAL:
                        cout << (*individuos_real)[i][j];
                        break;
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("\n");
    }

    void print_fitness(vector<double> fitness){
        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t%.4lf\n", i, fitness[i]);
        }
    }
};

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int tipo_variavel, tamanho_populacao, tamanho_cromossomo, numero_variaveis;
    string problema;
    double limite_inferior, limite_superior;
    vector<pair<double, double> > limites;

    if(argc != 2){
        printf("%s <arquivo_parametros>\n", argv[0]);
        return 1;
    }

    ifstream arquivo_parametros;
    arquivo_parametros.open(argv[1]);

    arquivo_parametros >> problema;
    arquivo_parametros >> tipo_variavel;
    arquivo_parametros >> tamanho_populacao;
    arquivo_parametros >> tamanho_cromossomo;
    arquivo_parametros >> numero_variaveis;
    
    for(int i = 0; i < numero_variaveis; i++){
        arquivo_parametros >> limite_inferior;
        arquivo_parametros >> limite_superior;
        limites.push_back(make_pair(limite_inferior, limite_superior));
    }

    Populacao populacao(tipo_variavel, tamanho_populacao, tamanho_cromossomo, &limites);

    populacao.gerar_populacao_inicial();
    populacao.print_populacao();
    populacao.fitness(problema);


}
