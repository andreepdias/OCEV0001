#ifndef __GA_CPP
#define __GA_CPP
using namespace std;

#include "avaliacao.cpp"
#include "selecao.cpp"
#include "crossover.cpp"
#include "mutacao.cpp"
#include "arquivos.cpp"

class GeneticAlgorithm{
private:
    Parametros *p;
    Arquivos *a;
    
    int execucao_atual;

    Avaliacao avaliacao;
    Selecao selecao;
    Crossover crossover;
    Mutacao mutacao;
    
    Populacao *populacao = NULL;
    Populacao *populacao_intermediaria = NULL;

public:
    
    GeneticAlgorithm(Parametros *_p, Arquivos *_a, int _e){
        p = _p;
        a = _a;

        execucao_atual = _e;

        populacao = new Populacao(p);
        populacao_intermediaria = new Populacao(p);

        avaliacao = Avaliacao(p, populacao);
        selecao = Selecao(p, populacao, populacao_intermediaria);
        crossover = Crossover(p, populacao_intermediaria);
        mutacao = Mutacao(p, populacao_intermediaria);

        (*populacao).gerar_populacao_aleatoria();
    }

    void loop_evolutivo(){
        for(int g = 0; g < (*p).numero_geracoes; g++){
            avaliacao.avaliacao();


            if((*p).elitismo and g > 0){
                elitismo();
            }

            if((*p).escalonamento_linear){
                escalonamento_linear(g);                
            }

            atualizar_grafico_convergencia(g);

            selecao.selecao();
            crossover.crossover();
            mutacao.mutacao();

            (*populacao) = (*populacao_intermediaria);
        }
    }
    
    void elitismo(){
        
        avaliacao.fitness_medio -= avaliacao.pior_individuo.second / (*p).tamanho_populacao;
        avaliacao.fitness_medio += (*populacao).melhor_individuo.fitness / (*p).tamanho_populacao;

        int indice_pior = avaliacao.pior_individuo.first;
        (*populacao).individuos[indice_pior] = (*populacao).melhor_individuo;

        if(avaliacao.melhor_individuo.second > (*populacao).melhor_individuo.fitness){
            int indice_melhor = avaliacao.melhor_individuo.first;
            (*populacao).melhor_individuo = (*populacao).individuos[indice_melhor];
        }else{
            avaliacao.melhor_individuo.second = (*populacao).melhor_individuo.fitness;
        }
    }

    void escalonamento_linear(int g){

        double fmax = avaliacao.melhor_individuo.second;
        double fmin = (*p).elitismo ? avaliacao.segundo_pior_individuo.second : avaliacao.pior_individuo.second;
        double favg = avaliacao.fitness_medio;

        double alpha = 0, beta = 0;

        double c = ((double(g) / ((*p).numero_execucoes - 1)) * 0.8) + 1.2;
        
        bool isFminGreater = false;

        if(fmin > ((c * favg) - fmax) / (c - 1)){
            isFminGreater = true;
        }

        if(isFminGreater){
            alpha = (favg * (c - 1)) / (fmax - favg);
            beta = (favg * (fmax - (c * favg))) / (fmax - favg);
        }else{
            alpha = favg / (favg - fmin);
            beta = (-fmin * favg) / (favg - fmin);
        }

        for(int i = 0; i < (*p).tamanho_populacao; i++){
            (*populacao).individuos[i].fitness = (alpha * (*populacao).individuos[i].fitness) + beta;
        }
    }

    void atualizar_grafico_convergencia(int g){
        g += 1;
        if(g % (*p).intervalo_plot == 0){
            double melhor_fitness = ((*p).elitismo and g > 0) ? (*populacao).melhor_individuo.fitness : avaliacao.melhor_individuo.second;
            printf("%d\t%.5lf\t%.5lf\t%.5lf\n",g, melhor_fitness, avaliacao.fitness_medio, avaliacao.pior_individuo.second);
            (*a).grafico_convergencia[execucao_atual] << g << " " << melhor_fitness << " " << avaliacao.fitness_medio << " " << avaliacao.pior_individuo.second << endl;
            (*a).grafico_convergencia[(*p).numero_execucoes] << g << " " << melhor_fitness << " " << avaliacao.fitness_medio << " " << avaliacao.pior_individuo.second << endl;
        }
    }


};

#endif