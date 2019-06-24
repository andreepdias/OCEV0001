#ifndef __ARQUIVOS_CPP
#define __ARQUIVOS_CPP
using namespace std;

#include "parametros.cpp"

class Arquivos{
public:

    Parametros *p;

    vector<ofstream> grafico_convergencia;
    vector<ofstream> grafico_diversidade;
    
    vector<ifstream> grafico_convergencia_read;
    vector<ifstream> grafico_diversidade_read;

    string caminho_convergencia;
    string caminho_diversidade;

    Arquivos(string _caminho_convergencia, string _caminho_diversidade, Parametros *_p){

        caminho_convergencia = _caminho_convergencia;
        caminho_diversidade = _caminho_diversidade;

        p = _p;

        grafico_diversidade.resize((*p).numero_execucoes + 1);
        grafico_convergencia.resize((*p).numero_execucoes + 1);
        
        grafico_diversidade_read.resize((*p).numero_execucoes + 1);
        grafico_convergencia_read.resize((*p).numero_execucoes + 1);
        
        for(int i = 0; i < (*p).numero_execucoes; i++){
            string cd = caminho_diversidade + "_" + to_string(i);
            grafico_diversidade[i].open(cd.c_str());
            
            string cc = caminho_convergencia + "_" + to_string(i);
            grafico_convergencia[i].open(cc.c_str());
        }
    }
    

    ~Arquivos(){
        for(int i = 0; i < (*p).numero_execucoes + 1; i++){
            grafico_convergencia[i].close();
            grafico_diversidade[i].close();
        }
    }

    void open(){
        grafico_convergencia[(*p).numero_execucoes].open(caminho_convergencia.c_str(), ios::trunc);
        grafico_diversidade[(*p).numero_execucoes].open(caminho_diversidade.c_str(), ios::trunc);
    }
    void close(){
        grafico_convergencia[(*p).numero_execucoes].close();
        grafico_diversidade[(*p).numero_execucoes].close();
    }

    void open_read(){
        for(int i = 0; i < (*p).numero_execucoes; i++){
            string cd = caminho_diversidade + "_" + to_string(i);
            grafico_diversidade_read[i].open(cd.c_str());
            
            string cc = caminho_convergencia + "_" + to_string(i);
            grafico_convergencia_read[i].open(cc.c_str());
        }
    }

    void close_read(){
        for(int i = 0; i < (*p).numero_execucoes + 1; i++){
            grafico_convergencia_read[i].close();
            grafico_diversidade_read[i].close();
        }
    }

};
#endif