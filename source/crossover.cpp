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
            case NCORTES_INTEIRO: break;
            case UNIFORME_BINARIO: break;
            case UNIFORME_INTEIRO: break;
            case BLX: break;
            case ARITMETICO: break;
            case MEDIA_UNIFORME: break;
            case PMX: pmx(); break;
        }
    }

    void pmx(){

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

};

#endif