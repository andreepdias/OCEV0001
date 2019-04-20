#ifndef __INDIVIDUOS_CPP
#define __INDIVIDUOS_CPP

#include "populacao.cpp"
using namespace std;



class Individuo{
private:

    double fitness;
    double funcao_objetivo;
    double infracao;

    vector<int> cromossomo;

public:
    Individuo(){
    }


};

#endif