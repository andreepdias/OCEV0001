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

    for(int i = 0; i < parametros.GEN; i++){
         populacao.Fitness();
         populacao.selecao();
         populacao.crossover();
         populacao.mutation();
    }
    populacao.Fitness();


}


// melhor, pior, média


#endif