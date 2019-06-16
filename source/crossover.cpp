#ifndef __CROSSOVER_CPP
#define __CROSSOVER_CPP
using namespace std;

#include "populacao.cpp"

class Crossover {
private:
    Parametros *p;
    
    Populacao *populacao;

public:
    Crossover(){}    
    Crossover(Parametros *_p, Populacao *_populacao){
        p = _p;
        populacao = _populacao;
    }

    void crossover(){
        switch((*p).tipo_crossover){
            case NCORTES_BINARIO: break;
            case NCORTES_INTEIRO: inteiro_n_cortes(); break;
            case UNIFORME_BINARIO: break;
            case UNIFORME_INTEIRO: inteiro_uniforme(); break;
            case BLX: break;
            case ARITMETICO: break;
            case MEDIA_UNIFORME: break;
            case PMX: inteiro_permutado_pmx(); break;
        }
    }

    void inteiro_permutado_pmx(){

        auto printIndiv = [&](int k){
            vector<int> *c = (vector<int>*)(*populacao).individuos[k].cromossomo;
            for(int i = 0; i < (*p).tamanho_cromossomo; i++){
                printf("%d ", (*c)[i]);
            }
            printf("\n");
        };

        mt19937 engine(random_device{}());

        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < (*p).tamanho_populacao; i += 2)
        {
            uniform_int_distribution<int> distribution{1, (*p).tamanho_cromossomo - 1};
            uniform_real_distribution<double> distribution_real{0, 1};

            vector<int> pontos_corte(2);
            map<int, int> matching_section_p1;
            map<int, int> matching_section_p2;


            double rd = distribution_real(engine);
            if(rd < (*p).probabilidade_crossover){

                matching_section_p1.clear();
                matching_section_p2.clear();

                pontos_corte[0] = distribution(engine);
                int r;
                do
                {
                    r = distribution(engine);
                } while (r == pontos_corte[0]);
                pontos_corte[1] = r;

                sort(pontos_corte.begin(), pontos_corte.end());

                vector<int> *c0 = (vector<int>*)(*populacao).individuos[i].cromossomo;
                vector<int> *c1 = (vector<int>*)(*populacao).individuos[i + 1].cromossomo;

                for (int k = pontos_corte[0]; k < pontos_corte[1]; k++)
                {

                    int aux = (*c0)[k];
                    (*c0)[k] = (*c1)[k];
                    (*c1)[k] = aux;

                    int e1 = (*c0)[k];
                    int e2 = (*c1)[k];

                    matching_section_p1[e1] = e2;
                    matching_section_p2[e2] = e1;
                }


                for (int j = 0; j < pontos_corte[0]; j++)
                {
                    int key = (*c0)[j];
                    while (matching_section_p1.find(key) != matching_section_p1.end())
                    {
                        key = matching_section_p1[key];
                    }
                    (*c0)[j] = key;

                    key = (*c1)[j];
                    while (matching_section_p2.find(key) != matching_section_p2.end())
                    {
                        key = matching_section_p2[key];
                    }
                    (*c1)[j] = key;
                }
                for (int j = pontos_corte[1]; j < (*p).tamanho_cromossomo; j++)
                {
                    int key = (*c0)[j];
                    while (matching_section_p1.find(key) != matching_section_p1.end())
                    {
                        key = matching_section_p1[key];
                    }
                    (*c0)[j] = key;

                    key = (*c1)[j];
                    while (matching_section_p2.find(key) != matching_section_p2.end())
                    {
                        key = matching_section_p2[key];
                    }
                    (*c1)[j] = key;
                }
            }
        }
    }

    void inteiro_n_cortes(){
        mt19937 engine(random_device{}());

        uniform_int_distribution<int> distribution{1, (*p).tamanho_cromossomo - 1};

        for(int i = 0; i < (*p).tamanho_populacao; i += 2)
        {
            vector<int> *c0 = (vector<int>*)(*populacao).individuos[i].cromossomo;
            vector<int> *c1 = (vector<int>*)(*populacao).individuos[i + 1].cromossomo;

            map<int, bool> indices_ponto_corte;
            vector<int> cortes_ordenados;
            for(int j = 0; j < (*p).ncortes_c; j++)
            {
                int ponto_corte_aleatorio;
                do
                {
                    ponto_corte_aleatorio = distribution(engine);

                } while (indices_ponto_corte.find(ponto_corte_aleatorio) != indices_ponto_corte.end());
                indices_ponto_corte[ponto_corte_aleatorio] = true;
                cortes_ordenados.push_back(ponto_corte_aleatorio);
            }        
            cortes_ordenados.push_back((*p).tamanho_cromossomo);
            sort(cortes_ordenados.begin(), cortes_ordenados.end());

            for(int j = 0; j < (*p).ncortes_c; j += 2)
            {
                for(int k = cortes_ordenados[j]; k < cortes_ordenados[j + 1]; k++)
                {
                    int aux = (*c0)[k];
                    (*c0)[k] = (*c1)[k];
                    (*c1)[k] = aux;
                }
            }
        }
    }

    void inteiro_uniforme(){
        mt19937 engine(random_device{}());

        uniform_int_distribution<int> distribution{0, 1};

        for(int i = 0; i < (*p).tamanho_populacao; i += 2)
        {
            vector<int> *c0 = (vector<int>*)(*populacao).individuos[i].cromossomo;
            vector<int> *c1 = (vector<int>*)(*populacao).individuos[i + 1].cromossomo;

            for(int j = 0; j < (*p).tamanho_cromossomo; j++)
            {
                int moeda = distribution(engine);
                if(moeda){
                    int aux = (*c0)[j];
                    (*c0)[j] = (*c1)[j];
                    (*c1)[j] = aux;
                }
            }
        }
    }

    void crossover_blx_real(double alpha = 0.5){
        mt19937 engine(random_device{}());

        for(int i = 0; i < (*p).tamanho_populacao; i += 2)
        {
            vector<double> *c0 = (vector<double>*)(*populacao).individuos[i].cromossomo;
            vector<double> *c1 = (vector<double>*)(*populacao).individuos[i + 1].cromossomo;

            for(int j = 0; j < (*p).tamanho_cromossomo; j++)
            {
                double maior_i = max((*c0)[j], (*c1)[j]);
                double menor_i = min((*c0)[j], (*c1)[j]);
                double d = maior_i - menor_i;
                maior_i += d*alpha;
                menor_i -= d*alpha;
                uniform_real_distribution<double> distribution{menor_i, maior_i};
                (*c0)[j] = distribution(engine);
                (*c1)[j] = distribution(engine);
            }
        }
    }

    void crossover_aritmetico_real(double alpha = 0.5){

        for(int i = 0; i < (*p).tamanho_populacao; i += 2)
        {
            vector<double> *c0 = (vector<double>*)(*populacao).individuos[i].cromossomo;
            vector<double> *c1 = (vector<double>*)(*populacao).individuos[i + 1].cromossomo;
            
            for(int j = 0; j < (*p).tamanho_cromossomo; j++)
            {
                double ind_temp_1 = alpha * (*c0)[j] + (1 - alpha) * (*c1)[j];
                double ind_temp_2 = (1 - alpha) * (*c0)[j] + alpha * (*c1)[j];
                (*c0)[j] = ind_temp_1;
                (*c1)[j] = ind_temp_2;
            }
        }
    }

};

#endif