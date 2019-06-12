#ifndef __ARQUIVOS_CPP
#define __ARQUIVOS_CPP
using namespace std;

#include "parametros.cpp"

class Arquivos{
public:

    Parametros *p;

    vector<ofstream> grafico_convergencia;
    vector<ofstream> grafico_diversidade;

    Arquivos(string caminho_convergencia, string caminho_diversidade, Parametros *_p){
        p = _p;

        grafico_diversidade.resize((*p).numero_execucoes + 1);
        grafico_convergencia.resize((*p).numero_execucoes + 1);
        
        grafico_diversidade[(*p).numero_execucoes].open(caminho_diversidade.c_str());
        grafico_convergencia[(*p).numero_execucoes].open(caminho_convergencia.c_str());
        
        for(int i = 0; i < (*p).numero_execucoes; i++){
            caminho_diversidade = caminho_diversidade + "_" + to_string(i);
            grafico_diversidade[i].open(caminho_diversidade.c_str());
            
            caminho_convergencia = caminho_convergencia + "_" + to_string(i);
            grafico_convergencia[i].open(caminho_convergencia.c_str());
        }
    }

    ~Arquivos(){
        for(int i = 0; i < (*p).numero_execucoes + 1; i++){
            grafico_convergencia[i].close();
            grafico_diversidade[i].close();
        }
    }

};
#endif