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

    vector<Individuo> melhores_individuos;
    Individuo melhor_individuo;

    for(int k = 0; k < (*parametros).numero_execucoes; k++){
    
        GeneticAlgorithm ga(parametros, arquivos, k);
        ga.loop_evolutivo();

        Individuo melhor_geracao = ga.melhor_individuo;
        melhores_individuos.push_back(melhor_geracao);
        printf("\nExecucao %d:\tFuncao Objetivo: %.5lf\t", k + 1, melhor_geracao.funcao_objetivo);
        melhor_geracao.print_restricoes();

        melhor_individuo = (melhor_geracao.fitness > melhor_individuo.fitness) ? melhor_geracao : melhor_individuo;
    }


    int n = melhores_individuos.size();
    double media = 0;
    for(int i = 0; i < n; i++){
        media += melhores_individuos[i].funcao_objetivo;
    }
    media /= n;

    double desvio = 0;
    for(int i = 0; i < n; i++){
        desvio += pow(media - melhores_individuos[i].funcao_objetivo, 2);
    }
    desvio /= n;
    desvio = sqrt(desvio);

    printf("\nTodas execucoes:\tFuncao Objetivo: %.5lf\t", melhor_individuo.funcao_objetivo);
    melhor_individuo.print_restricoes();
    printf("\nMedia: %.5lf\t\tDesvio Padrao: %.5lf\n\n", media, desvio);
    melhor_individuo.print_cromossomo();

    (*arquivos).open_read();
    (*arquivos).open();
    for(int i = 0; i < (*parametros).numero_geracoes / (*parametros).intervalo_plot; i++){

        int g;
        double media_melhor = 0;
        double media_pior = 0;
        double media_media = 0;
        double p, m, me;
        for(int k = 0; k < (*parametros).numero_execucoes; k++){
            (*arquivos).grafico_convergencia_read[k] >> g >> m >> p >> me;
            media_melhor += m;
            media_pior += p;
            media_media += me;
        }
        media_melhor /= (*parametros).numero_execucoes;
        media_pior  /= (*parametros).numero_execucoes;
        media_media /= (*parametros).numero_execucoes;

        (*arquivos).grafico_convergencia[(*parametros).numero_execucoes] << g << " " << media_melhor << " " << media_pior << " " << media_media << endl;

    }
    (*arquivos).close_read();
    (*arquivos).close();

    delete parametros;
    delete arquivos;
}

#endif
