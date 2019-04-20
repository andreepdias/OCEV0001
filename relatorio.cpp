#ifndef __RELATORIO_CPP
#define __RELATORIO_CPP

#include <vector>
#include <string>
#include <utility>

using namespace std;

class Relatorio{
public:
    double fitness;
    double funcao_objetivo;
    double infracao_restricoes;

    vector <pair<string, double> >  variaveis;

    Relatorio(){}

    Relatorio(double _f, double _fo, double _ir, vector <pair<string, double> >  v){
        fitness = _f;
        funcao_objetivo = _fo;
        infracao_restricoes = _ir;

        variaveis = v;
    }
};


#endif