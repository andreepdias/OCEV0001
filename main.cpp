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

    void selecao_roleta(){
        random_device device{};
        mt19937 engine{device()};
        uniform_real_distribution<double> distribution{0.0, 1.0};

        vector<double> fitness_relativo(tamanho_populacao);
        double somatorio, somatorio_atual, soma_acumulada, r;

        somatorio = 0.0;
        for(int i = 0; i < tamanho_populacao; i++){
            somatorio += (*fitness)[i];
        }

        int k = 0, individuo_selecionado = -1;
        for(int x = 0; x < tamanho_populacao; x++){

            somatorio_atual = somatorio;
            if(individuo_selecionado != -1){
                somatorio_atual -= (*fitness)[individuo_selecionado];
            }
            somatorio_atual = somatorio_atual == 0 ? 1 : somatorio_atual;

            for(int i = 0; i < tamanho_populacao; i++){
                if(i == individuo_selecionado) continue;
                fitness_relativo[i] = (*fitness)[i] / somatorio_atual;
            }

            soma_acumulada = 0.0;
            r = distribution(engine);

            for(int i = 0; i < tamanho_populacao; i++){
                if(i == individuo_selecionado) continue;
                if (fitness_relativo[i] + soma_acumulada >=  r){
                    individuos_selecionados[k++] = i;
                    individuo_selecionado = i;
                    break;
                }
                soma_acumulada += fitness_relativo[i];
            }
        }
    }

    void selecao_ranking() {
        random_device device{};
        mt19937 engine{device()};
        uniform_real_distribution<double> distribution{0.0, 1.0};

        vector<pair<double, int> > ranking_individuos(tamanho_populacao);
        vector<double> fitness_relativo(tamanho_populacao);

        for(int i = 0; i < tamanho_populacao; i++){
            ranking_individuos[i] = make_pair((*fitness)[i], i);
        }
        sort(ranking_individuos.begin(), ranking_individuos.end());

        double somatorio = double(tamanho_populacao * (tamanho_populacao + 1)) / 2;
        for (int i = 0; i < tamanho_populacao; i++){
            fitness_relativo[ranking_individuos[i].second] = double(i + 1) / somatorio;
            // printf("Raking %i:\ti:%d\tfit:%lf\n", i + 1, ranking_individuos[i].second, ranking_individuos[i].first);
        }

        double soma_acumulada, r;
        int k = 0;
        for (int x = 0; x < tamanho_populacao; x++){
            soma_acumulada = 0.0;
            r = distribution(engine);

            for (int i = 0; i < tamanho_populacao; i++){
                if (fitness_relativo[i] + soma_acumulada >= r){
                    individuos_selecionados[k++] = i;
                    break;
                }
                soma_acumulada += fitness_relativo[i];
            }
        }
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

    void print_fitness(){
        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t%.4lf\n", i, (*fitness)[i]);
        }
        printf("\n");
    }
    void print_selecionados(){
        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t%d\n", i, individuos_selecionados[i]);
        }
        printf("\n");
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
    populacao.Fitness(problema);
    populacao.selecao_ranking();
    populacao.print_selecionados();


}
