#ifndef __PARAMETROS_CPP
#define __PARAMETROS_CPP

#include <fstream>
#include <iostream>
using namespace std;

class Parametros{
public:
    int COD;
    int RUN;
    int GEN;
    int POP;
    int DIM;

    double PC;
    double PM;

    bool ELIT;

    int COD_SELECTION;
    int kk; double kp;
    int t, d;

    int COD_CROSSOVER;
    int c;
    int COD_MUTATION;

    int PROBLEM;

    string carregar_parametros(string arquivo){

        ifstream arquivo_parametros;
        arquivo_parametros.open(arquivo);

        string retorno, k, v;

        while(arquivo_parametros >> k >> v){
            if(k == "COD"){
                printf("Tipo de dominio:\n");
                if(v == "BIN"){
                    COD = 0;
                    printf("\tBinario\n");
                }else if(v == "INT"){
                    COD = 1;
                    printf("\tInteiro\n");
                }else if(v == "INT-PERM"){
                    COD = 2;
                    printf("\tInteiro Permutado\n");
                }else if(v == "REAL"){
                    COD = 3;
                    printf("\tReal\n");
                }else{
                    retorno += "Tipo de variavel desconhecida.\n";
                }
            }else if(k == "RUN"){
                RUN = stoi(v);
                printf("Numero de execucoes:\n");
                printf("\t%d\n", RUN);
            }else if(k == "GEN"){
                GEN = stoi(v);
                printf("Numero de geracoes:\n");
                printf("\t%d\n", GEN);
            }else if(k == "POP"){
                POP = stoi(v);
                printf("Tamanho populacao:\n");
                printf("\t%d\n", POP);
            }else if(k == "DIM"){
                DIM = stoi(v);
                printf("Numero do cromossomo:\n");
                printf("\t%d\n", DIM);
            }else if(k == "PC"){
                PC = stod(v);
                printf("Probabilidade de crossover:\n");
                printf("\t%lf\n", PC);
            }else if(k == "PM"){
                PM = stod(v);
                printf("Probabilidade de mutacao:\n");
                printf("\t%lf\n", PM);
            }else if(k == "ELIT"){
                ELIT = stoi(v);
                printf("Elitismo:\n");
                printf("\t%d\n", ELIT);
            }

            else if(k == "COD-SELECTION"){
                double x;
                printf("Tipo de selecao:\n");
                if(v == "ROLETA"){
                    COD_SELECTION = 1;
                    printf("\tRoleta\n");
                }else if(v == "RANKING"){
                    COD_SELECTION = 2;
                    printf("\tRanking\n");
                }else if(v == "TORNEIO"){
                    COD_SELECTION = 3;
                    
                    string x;

                    arquivo_parametros >> x;
                    kk = stoi(x);
                    arquivo_parametros >> x;
                    kp = stof(x);

                    printf("\tTorneio\tk:%d\tkp:%lf\n", kk, kp);
                }else if(v == "VIZINHANCA"){
                    COD_SELECTION = 4;
                    string t; int d;

                    printf("\tVizinhanca\t");
                    arquivo_parametros >> t;
                    if(t == "MELHOR"){
                        t = 1;
                        printf("melhor\t");
                    }else if(t == "ALEATORIO"){
                        t = 2;
                        printf("aleatorio\t");
                    }else if(t == "PROPORCIONAL"){
                        t = 3;
                        printf("porporcional\t");
                    }else{
                        retorno += "Tipo de Selecao por vizinhanca nao encontrada.\n";
                    }
                    arquivo_parametros >> d;
                    d = d;
                    printf("d; %d\n", d);
                }else{
                    retorno += "Tipo de Selecao nao encontrada.\n";
                }
            }
            
            else if(k == "COD-CROSSOVER"){
                printf("Tipo crossover:\n");
                if(v == "NCORTES"){
                    if(COD == 0){
                        COD_CROSSOVER = 1;
                    }else if(COD == 1){
                        COD_CROSSOVER = 2;
                    }else{
                        retorno += "Defina o tipo de variavel antes de escolher o Crossover NCortes\n";
                    }
                    int x;
                    arquivo_parametros >> x;
                    c = x;
                    printf("\tNCortes\tc:%d\n", c);
                }else if(v == "UNIFORME"){
                    if(COD == 0){
                        COD_CROSSOVER = 3;
                    }else if(COD == 1){
                        COD_CROSSOVER = 4;
                    }else{
                        retorno += "Defina o tipo de variavel antes de escolher o Crossover Uniforme\n";
                    }
                    printf("\tUniforme\n");
                }else if(v == "BLX"){
                    COD_CROSSOVER = 5;
                    printf("\tBLX\n");
                }else if(v == "ARITMETICO"){
                    COD_CROSSOVER = 6;
                    printf("\tAritimetico\n");
                }else if(v == "MEDIA-UNIFORME"){
                    COD_CROSSOVER = 7;
                    printf("\tMedia Uniforme\n");
                }else if(v == "PMX"){
                    COD_CROSSOVER = 8;
                    printf("\tPMX\n");
                }
            }

            else if(k == "COD-MUTATION"){
                printf("Tipo mutacao:\n");
                if(v == "BIT-FLIP"){
                    COD_MUTATION = 1;
                    printf("\tBit-Flip\n");
                }else if(v == "DELTA"){
                    COD_MUTATION = 2;
                    printf("\tDelta\n");
                }else if(v == "SWAP"){
                    COD_MUTATION = 3;
                    printf("\tSwap\n");
                }else{
                    retorno += "Tipo de Mutacao nao encontrado\n";
                }
            }

            else if(k == "PROBLEM"){
                printf("Problema:\n");
                if(v == "NQUEENS"){
                    PROBLEM = 1;
                    printf("NQueens\n");
                }
            }
        }
        return retorno;
    }
};
#endif