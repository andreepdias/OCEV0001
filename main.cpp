#include <bits/stdc++.h>
#include "populacao.cpp"
using namespace std;


int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int tipo_variavel, tamanho_populacao, tamanho_cromossomo, numero_variaveis, tipo_selecao;
    double limite_inferior, limite_superior;
    string problema;

    vector<pair<double, double> > limites;

    int k, d, tipo_vizinhanca;
    double kp;
    pair<int, double> parametros_torneio;
    pair<int, int> parametros_vizinhanca;

    if(argc < 3){
        printf("%s <arquivo_parametros> <tipo_selecao>\n\n<tipo_selecao>:\n1: Roleta sem reposicao\n2: Ranking Uniforme\n3: Torneio Estocastico\n4: Vizinhanca Linear\n", argv[0]);
        return 1;
    }

    tipo_selecao = atoi(argv[2]);
    if(tipo_selecao == 3){
        if(argc != 5){
            k = 2;
            kp = 1.0;
        }else{
            k = atoi(argv[3]);
            kp = atof(argv[4]);
        }
        parametros_torneio = make_pair(k, kp);
        printf("Selecao Torneio escolhida. Parametros: k = %d e kp = %lf.\n", k, kp);
    }else if(tipo_selecao == 4){
        if(argc != 5){
            d = 2;
            tipo_vizinhanca = 1;
        }else{
            d = atoi(argv[3]);
            tipo_vizinhanca = atoi(argv[4]);
        }
        parametros_vizinhanca = make_pair(d, tipo_vizinhanca);
        printf("Selecao Vizinhanca escolhida. Parametros: d = %d e tipo = %d.\n", d, tipo_vizinhanca);
    }

    void *parametros_selecao;
    switch(tipo_selecao){
        case 1: parametros_selecao = NULL; break;
        case 2: parametros_selecao = NULL; break;
        case 3: parametros_selecao = (void*)&parametros_torneio; break;
        case 4: parametros_selecao = (void*)&parametros_vizinhanca; break;
    }
    // printf("aaaaa\n");
    ifstream arquivo_parametros;
    arquivo_parametros.open(argv[1]);

    arquivo_parametros >> problema;
    arquivo_parametros >> tipo_variavel;
    arquivo_parametros >> tamanho_populacao;
    arquivo_parametros >> tamanho_cromossomo;
    arquivo_parametros >> numero_variaveis;

    // printf("b\n");
    for(int i = 0; i < numero_variaveis; i++){
        arquivo_parametros >> limite_inferior;
        // printf("c\n");
        arquivo_parametros >> limite_superior;
        // printf("d\n");
        limites.push_back(make_pair(limite_inferior, limite_superior));
    }
    // printf("e\n");

    Populacao populacao(tipo_variavel, tamanho_populacao, tamanho_cromossomo, &limites);

    populacao.gerar_populacao_inicial();

    for(int i = 0; i < 1000; i++){
        populacao.Fitness(problema);
        populacao.selecao(tipo_selecao, parametros_selecao, true);
        populacao.crossover_pmx_intp();
        populacao.swap_mutation(0.05);
    }


}


// melhor, pior, média
