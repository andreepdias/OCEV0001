#ifndef __MUTACAO_CPP
#define __MUTACAO_CPP
using namespace std;

#include "populacao.cpp"
#include "util.cpp"

class Mutacao {
private:
    Parametros *p;
    
    Populacao *populacao;

public:
    Mutacao(){}    
    Mutacao(Parametros *_p, Populacao *_populacao){
        p = _p;
        populacao = _populacao;
    }

    void mutacao(){
        switch((*p).tipo_mutacao){
            case BIT_FLIP:                                          break;
            case DELTA:                                             break;
            case SWAP:                  inteiro_permutado_swap();   break;
            case INTEIRA:               inteiro_inteira();          break;
            case MIKA: real_michalewicz();         break;
        }
    }

    void inteiro_permutado_swap()
    {
        uniform_real_distribution<double> distribution_real{0.0, 1.0};
        uniform_int_distribution<int> distribution_int(0, (*p).tamanho_cromossomo - 1);

        mt19937 engine(random_device{}());
        
        #pragma omp parallel for schedule(dynamic)        
        for(int i = 0; i < (*p).tamanho_populacao; i++){
            double r;
            int g;

            vector<int> *c = (vector<int>*)(*populacao).individuos[i].cromossomo;

            for(int j = 0; j < (*p).tamanho_cromossomo; j++){

                r = distribution_real(engine);
                
                if(r < (*p).probabilidade_mutacao){
                
                    do{
                        g = distribution_int(engine);
                    }while(g == j);
                
                    int aux = (*c)[j];
                    (*c)[j] = (*c)[g];
                    (*c)[g] = aux;
                }
            }
        }

    }

    void inteiro_inteira(){
        mt19937 engine(random_device{}());

        uniform_real_distribution<double> distribution_real{0.0, 1.0};
        uniform_int_distribution<int> distribution_int((*p).limite_inferior, (*p).limite_superior);
        double r;

        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < (*p).tamanho_populacao; i++)
        {
            vector<int> *c = (vector<int>*)(*populacao).individuos[i].cromossomo;

            for (int j = 0; j < (*p).tamanho_cromossomo; j++)
            {
                r = distribution_real(engine);
                if (r <= (*p).probabilidade_mutacao)
                {
                    int random;
                    do{
                        random = distribution_int(engine);
                    }while(random == (*c)[j]);
                    
                    (*c)[j] = random;
                }
            }
        }
    }

    void real_michalewicz(){
        mt19937 engine(random_device{}());
        uniform_real_distribution<double> distribution_real{0.0, 1.0};
        uniform_int_distribution<int> distribution_int(0, 1);

        auto delta = [&](double y, double a, double b) {
            double t = (*populacao).geracao + 1;
            double T = (*p).numero_geracoes;

            // double delta = y * (1.0 - std::pow(a, std::pow(1.0 - geracao_atual / (double)config.getGenerations(), config.getB())));

            //  return y * (1.0 - pow(a, (1.0 - ((pow(t, 1.0 / (*p).mutacao_mika) / pow(T, 1.0 / (*p).mutacao_mika))) ) * b) ); 
            //return y * (1.0 - pow(a, pow(1.0 - pow(t, 1.0 / (*p).mutacao_mika) / pow(T, 1.0 / (*p).mutacao_mika), b     ) ) ); // bruno

            return y * (1.0 - pow(a, pow((1.0 - pow(t, 1.0 / (*p).mutacao_mika) / pow(T, 1.0 / (*p).mutacao_mika)), b)));

        };

        #pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < (*p).tamanho_populacao; i++){

            vector<double> *c = (vector<double>*)(*populacao).individuos[i].cromossomo;

            for(int j = 0; j < (*p).tamanho_cromossomo; j++){

                double r = distribution_real(engine);
                if(r <= (*p).probabilidade_mutacao){

                    double a = distribution_real(engine);
                    double b = 5;

                    double bit = distribution_real(engine);

                    if(bit >= 0.50){
                        (*c)[j] = (*c)[j] + delta((*p).limite_superior - (*c)[j], a, b);
                    }else{
                        (*c)[j] = (*c)[j] - delta((*c)[j] - (*p).limite_inferior, a, b);
                    }
                }
            }
        }
    }

};

#endif