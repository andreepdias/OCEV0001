#ifndef __MAIN_CPP
#define __MAIN_CPP
using namespace std;



#include "parametros.cpp"
#include "arquivos.cpp"
#include "ga.cpp"

int main(int argc, char const *argv[])
{
    if (argc < 2){
        printf("%s <arquivo_parametros>\n", argv[0]);
    }
    
    /* Declaração e leitura arquivo de parâmaetros */
    Parametros *parametros = new Parametros();    
    string mensagem_erro = (*parametros).carregar_parametros(argv[1]);
    if (mensagem_erro.size() > 0){
        cout << mensagem_erro << endl;
        return EXIT_FAILURE;
    }

    Arquivos *arquivos = new Arquivos("saidas/grafico_convergencia", "saidas/grafico_diversidade", parametros);

    for(int k = 0; k < (*parametros).numero_execucoes; k++){
        GeneticAlgorithm ga(parametros, arquivos, k);
        ga.loop_evolutivo();
    }
    free(parametros);
    free(arquivos);
}

#endif
