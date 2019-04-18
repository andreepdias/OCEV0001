#ifndef __MAIN_CPP
#define __MAIN_CPP

#include "populacao.cpp"

using namespace std;


int main(int argc, char const *argv[])
{
    
    if(argc < 2){
        printf("%s <arquivo_parametros>\n", argv[0]);
    }

    Parametros parametros;
    string r = parametros.carregar_parametros(argv[1]);
    if(r.size() > 0){
        cout << r << endl;
        return 1;
    }

    Populacao populacao(parametros);
    populacao.gerar_populacao_inicial();

    ofstream out;
    out.open("tempos");

    for(int i = 0; i < parametros.GEN; i++){
        // printf("----------------------------Iteracao %d----------------------------\n", i + 1);
        populacao.Fitness(i, out);
        // printf("--------------Fitness:--------------\n");
        // populacao.print_populacao();

        populacao.selecao();
        // printf("--------------Selecao:--------------\n");
        // populacao.print_selecionados();
        // populacao.print_populacao();

        populacao.crossover();
        // printf("--------------Crossover:--------------\n");
        // populacao.print_populacao();

        populacao.mutation();
        // printf("--------------Mutacao::--------------\n");
        // populacao.print_populacao();
    }
    populacao.Fitness(parametros.GEN, out);
    // populacao.print_finalizacao_execucao();
}


// melhor, pior, média


#endif