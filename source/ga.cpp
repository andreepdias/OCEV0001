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
    Populacao *populacao_crowding = NULL;

public:
    Individuo melhor_individuo;
    
    GeneticAlgorithm(Parametros *_p, Arquivos *_a, int _e){
        p = _p;
        a = _a;

        execucao_atual = _e;

        populacao = new Populacao(p);
        populacao_intermediaria = new Populacao(p);
        populacao_crowding = new Populacao(p);

        avaliacao = Avaliacao(p, populacao);
        selecao = Selecao(p, populacao, populacao_intermediaria);
        crossover = Crossover(p, populacao_intermediaria);
        mutacao = Mutacao(p, populacao_intermediaria);
        (*populacao).gerar_populacao_aleatoria();

    }

    void loop_evolutivo(){
        
        avaliacao.avaliacao();

        (*a).open();

        for(int g = 0; g < (*p).numero_geracoes; g++){
            (*populacao).geracao = g;
            (*populacao_intermediaria).geracao = g;


            if((g + 1) % (*p).intervalo_plot == 0){
                atualizar_grafico_convergencia(g + 1);
                // avaliacao.print_melhor_individiduo();
            }

            if((*p).escalonamento_linear){
                escalonamento_linear(g);                
            }

            selecao.selecao();
            mutacao.mutacao();

            if((*p).tipo_crowding != 0){
                crowding();
            }

            if((*p).genereation_gap > 0.0){
                generation_gap();
            }else{
                (*populacao) = (*populacao_intermediaria);
            }

            avaliacao.avaliacao();

            if((*p).elitismo){
                elitismo();
            }
        }
        
        (*a).close();

        melhor_individuo = (*populacao).melhor_individuo;

        delete populacao;
        delete populacao_intermediaria;
        delete populacao_crowding;
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

        avaliacao.pior_individuo.first = avaliacao.segundo_pior_individuo.first;
        avaliacao.pior_individuo.second = avaliacao.segundo_pior_individuo.second;
    }

    void escalonamento_linear(int g){

        double fmax = avaliacao.melhor_individuo.second;
        double fmin = avaliacao.pior_individuo.second;
        double favg = avaliacao.fitness_medio;

        double alpha = 0, beta = 0;

        double c = ((double(g) / ((*p).numero_geracoes - 1)) * 0.8) + 1.2;
        
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
        double melhor_fitness = ((*p).elitismo and g > 1) ? (*populacao).melhor_individuo.fitness : avaliacao.melhor_individuo.second;
        // printf("gen: %d\tbest: %.10lf\tavg: %.5lf\tworst: %.5lf\n",g, melhor_fitness, avaliacao.fitness_medio, avaliacao.pior_individuo.second);
        (*a).grafico_convergencia[execucao_atual] << g << " " << melhor_fitness << " " << avaliacao.fitness_medio << " " << avaliacao.pior_individuo.second << endl;
        (*a).grafico_convergencia[(*p).numero_execucoes] << g << " " << melhor_fitness << " " << avaliacao.fitness_medio << " " << avaliacao.pior_individuo.second << endl;
    }

    void generation_gap(){

        int gap = (int) (double((*p).genereation_gap) * (*p).tamanho_populacao);

        for(int i = 0; i < gap; i++){
            (*populacao).individuos[i] = (*populacao_intermediaria).individuos[i];
        }
    }

    void crowding(){
        switch((*p).tipo_crossover){
            case STANDARD_CROWDING:
                standard_crowding();
                break;
            case DETERMINISTIC_CROWDING:
                deterministic_crowding();
                break;
        }
    }

    double calcula_distancia_real(Individuo &a, Individuo &b){
        vector<double> *ca = (vector<double>*)a.cromossomo;
        vector<double> *cb = (vector<double>*)b.cromossomo;

        double distancia = 0;

        for(int i = 0; i < (*p).tamanho_cromossomo; i++){
            distancia += abs((*ca)[i] - (*cb)[i]);
        }

        return distancia;
    }

    double calcula_distancia(Individuo &a, Individuo &b){
        switch((*p).tipo_variavel){
            case REAL:
                return calcula_distancia_real(a, b);
                break;
        }
        return 0;
    }

    void standard_crowding(){
        mt19937 engine(random_device{}());
        uniform_int_distribution<int> distribution_int(0, (*p).tamanho_populacao - 1);

        vector<bool> pais_substituidos((*p).tamanho_populacao, false);

        for(int x = 0; x < (*p).tamanho_populacao; x++){

            map<int, bool> ja_escolhidos;
            ja_escolhidos[x] = true;
            vector<pair<int, double> > fc_escolhidos;

            for(int k = 0; k < (*p).standard_crowding_fc; k++){

                int r;
                do{
                    r = distribution_int(engine);
                }while(ja_escolhidos.find(r) != ja_escolhidos.end());

                fc_escolhidos.push_back(make_pair(r, calcula_distancia((*populacao).individuos[r], (*populacao_intermediaria).individuos[x] ) ) );
            }

            pair<int, double> individuo_menor_distancia = fc_escolhidos[0];

            for(int i = 1; i < (*p).standard_crowding_fc; i++){
                if(fc_escolhidos[i].second < individuo_menor_distancia.second){
                    individuo_menor_distancia = fc_escolhidos[i];
                }
            }

            if(!pais_substituidos[individuo_menor_distancia.first]){
                (*populacao_crowding).individuos[individuo_menor_distancia.first] = (*populacao_intermediaria).individuos[x];
                pais_substituidos[individuo_menor_distancia.first] = true;
            }else{
                if((*populacao_intermediaria).individuos[x].fitness > (*populacao_crowding).individuos[individuo_menor_distancia.first].fitness){
                    (*populacao_crowding).individuos[individuo_menor_distancia.first] = (*populacao_intermediaria).individuos[x];
                }
            }
        }

        for(int x = 0; x < (*p).tamanho_populacao; x++){
            if(!pais_substituidos[x]){
                (*populacao_crowding).individuos[x] = (*populacao).individuos[x];
            }
        }

        (*populacao_intermediaria) = (*populacao_crowding);
        
    }

    void deterministic_crowding(){

        #pragma omp parallel for schedule(dynamic)
        for(int x = 0; x < (*p).tamanho_populacao; x += 2){

            double d1 = calcula_distancia((*populacao_intermediaria).individuos[x], (*populacao).individuos[x]) 
                + calcula_distancia((*populacao_intermediaria).individuos[x + 1], (*populacao).individuos[x + 1]);
            
            double d2 = calcula_distancia((*populacao_intermediaria).individuos[x], (*populacao).individuos[x + 1]) 
                + calcula_distancia((*populacao_intermediaria).individuos[x + 1], (*populacao).individuos[x]);

            if(d1 < d2){
                if((*populacao_intermediaria).individuos[x].fitness > (*populacao).individuos[x].fitness){
                    (*populacao_crowding).individuos[x] = (*populacao_intermediaria).individuos[x];
                }else{
                    (*populacao_crowding).individuos[x] = (*populacao).individuos[x];
                }

                if((*populacao_intermediaria).individuos[x + 1].fitness > (*populacao).individuos[x + 1].fitness){
                    (*populacao_crowding).individuos[x + 1] = (*populacao_intermediaria).individuos[x + 1];
                }else{
                    (*populacao_crowding).individuos[x + 1] = (*populacao).individuos[x + 1];
                }
            }else{
                if((*populacao_intermediaria).individuos[x + 1].fitness > (*populacao).individuos[x].fitness){
                    (*populacao_crowding).individuos[x] = (*populacao_intermediaria).individuos[x + 1];
                }else{
                    (*populacao_crowding).individuos[x] = (*populacao).individuos[x];
                }

                if((*populacao_intermediaria).individuos[x].fitness > (*populacao).individuos[x + 1].fitness){
                    (*populacao_crowding).individuos[x + 1] = (*populacao_intermediaria).individuos[x];
                }else{
                    (*populacao_crowding).individuos[x + 1] = (*populacao).individuos[x + 1];
                }
            }

        }

        (*populacao_intermediaria) = (*populacao_crowding);

    }


};

#endif