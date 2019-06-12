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
            case BIT_FLIP:          break;
            case DELTA:             break;
            case SWAP:      swap(); break;
            case INTEIRA:           break;
        }
    }

    void swap()
    {
        uniform_real_distribution<double> distribution_real{0.0, 1.0};
        uniform_int_distribution<int> distribution_int(0, (*p).tamanho_cromossomo - 1);

        mt19937 engine(random_device{}());
        

        #pragma omp parallel for schedule(dynamic)        
        for(int i = 0; i < (*p).tamanho_populacao; i++){
            vector<int> *c;
            double r;
            int g;

            c = (vector<int>*)(*populacao).individuos[i].cromossomo;

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

};

#endif