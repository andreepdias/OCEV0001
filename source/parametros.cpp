#ifndef __PARAMETROS_CPP
#define __PARAMETROS_CPP
using namespace std;

#include "util.cpp"

class Parametros{
public:
    int tipo_variavel;
    int numero_execucoes;
    int numero_geracoes;
    int tamanho_populacao;
    int tamanho_cromossomo;

    double probabilidade_crossover;
    double probabilidade_mutacao;

    int limite_inferior, limite_superior;

    bool elitismo;
    
    bool escalonamento_linear;

    int problema;
    int tipo_selecao;
    int tipo_crossover;
    int tipo_mutacao;

    int torneio_k; double torneio_kp;
    int vizinhanca_t, vizinhanca_d;

    int ncortes_c;

    int intervalo_plot;

    Parametros(){}

    string carregar_parametros(string arquivo){

        ifstream arquivo_parametros;
        arquivo_parametros.open(arquivo);

        string retorno, k, v;

        while(arquivo_parametros >> k >> v){
            if(k == "COD"){
                printf("Tipo de dominio:\n");
                if(v == "BIN"){
                    tipo_variavel = 0;
                    printf("\tBinario\n");
                }else if(v == "INT"){
                    tipo_variavel = 1;
                    printf("\tInteiro\n");
                }else if(v == "INT-PERM"){
                    tipo_variavel = 2;
                    printf("\tInteiro Permutado\n");
                }else if(v == "REAL"){
                    tipo_variavel = 3;
                    printf("\tReal\n");
                }else{
                    retorno += "Tipo de variavel desconhecida.\n";
                }
            }else if(k == "RUN"){
                numero_execucoes = stoi(v);
                printf("Numero de execucoes:\n");
                printf("\t%d\n", numero_execucoes);
            }else if(k == "GEN"){
                numero_geracoes = stoi(v);
                printf("Numero de geracoes:\n");
                printf("\t%d\n", numero_geracoes);
            }else if(k == "POP"){
                tamanho_populacao = stoi(v);
                printf("Tamanho populacao:\n");
                printf("\t%d\n", tamanho_populacao);
            }else if(k == "DIM"){
                tamanho_cromossomo = stoi(v);
                printf("Numero do cromossomo:\n");
                printf("\t%d\n", tamanho_cromossomo);
            }else if(k == "PC"){
                probabilidade_crossover = stod(v);
                printf("Probabilidade de crossover:\n");
                printf("\t%lf\n", probabilidade_crossover);
            }else if(k == "PM"){
                probabilidade_mutacao = stod(v);
                printf("Probabilidade de mutacao:\n");
                printf("\t%lf\n", probabilidade_mutacao);
            }else if(k == "ELIT"){
                elitismo = stoi(v);
                printf("Elitismo:\n");
                printf("\t%d\n", elitismo);
            }

            else if(k == "COD-SELECTION"){
                double x;
                printf("Tipo de selecao:\n");
                if(v == "ROLETA"){
                    tipo_selecao = 21;
                    printf("\tRoleta\n");
                }else if(v == "RANKING"){
                    tipo_selecao = 22;
                    printf("\tRanking\n");
                }else if(v == "TORNEIO"){
                    tipo_selecao = 23;
                    
                    string x;

                    arquivo_parametros >> x;
                    torneio_k = stoi(x);
                    arquivo_parametros >> x;
                    torneio_kp = stof(x);
                        printf("\tTorneio\tk:%d\tkp:%lf\n", torneio_k, torneio_kp);
                }else if(v == "VIZINHANCA"){
                    tipo_selecao = 24;
                    string tipo; int valor_d;

                    printf("\tVizinhanca\t");
                    arquivo_parametros >> tipo;
                    if(tipo == "MELHOR"){
                        vizinhanca_t = 1;
                        printf("melhor %d\t", vizinhanca_t);
                    }else if(tipo == "ALEATORIO"){
                        vizinhanca_t = 2;
                        printf("aleatorio\t");
                    }else if(tipo == "PROPORCIONAL"){
                        vizinhanca_t = 3;
                        printf("porporcional\t");
                    }else{
                        retorno += "Tipo de Selecao por vizinhanca nao encontrada.\n";
                    }
                    arquivo_parametros >> valor_d;
                    vizinhanca_d = valor_d;
                    printf("d: %d\n", vizinhanca_d);
                }else{
                    retorno += "Tipo de Selecao nao encontrada.\n";
                }
            }
            
            else if(k == "COD-CROSSOVER"){
                printf("Tipo crossover:\n");
                if(v == "NCORTES"){
                    if(tipo_variavel == 0){
                        tipo_crossover = 31;
                    }else if(tipo_variavel == 1){
                        tipo_crossover = 32;
                    }else{
                        retorno += "Defina o tipo de variavel antes de escolher o Crossover NCortes\n";
                    }
                    int x;
                    arquivo_parametros >> x;
                    ncortes_c = x;
                    printf("\tNCortes\tc:%d\n", ncortes_c);
                }else if(v == "UNIFORME"){
                    if(tipo_variavel == 0){
                        tipo_crossover = 33;
                    }else if(tipo_variavel == 1)
                    {
                        tipo_crossover = 34;
                    }else{
                        retorno += "Defina o tipo de variavel antes de escolher o Crossover Uniforme\n";
                    }
                    printf("\tUniforme\n");
                }else if(v == "BLX"){
                    tipo_crossover = 35;
                    printf("\tBLX\n");
                }else if(v == "ARITMETICO"){
                    tipo_crossover = 36;
                    printf("\tAritimetico\n");
                }else if(v == "MEDIA-UNIFORME"){
                    tipo_crossover = 37;
                    printf("\tMedia Uniforme\n");
                }else if(v == "PMX"){
                    tipo_crossover = 38;
                    printf("\tPMX\n");
                }
            }

            else if(k == "COD-MUTATION"){
                printf("Tipo mutacao:\n");
                if(v == "BIT-FLIP"){
                    tipo_mutacao = 41;
                    printf("\tBit-Flip\n");
                }else if(v == "DELTA"){
                    tipo_mutacao = 42;
                    printf("\tDelta\n");
                }else if(v == "SWAP"){
                    tipo_mutacao = 43;
                    printf("\tSwap\n");
                }else if(v == "INTEIRA"){
                    tipo_mutacao = 44;
                    printf("\tInteira\n");
                }else{
                    retorno += "Tipo de Mutacao nao encontrado\n";
                }
            }

            else if(k == "PROBLEM"){
                printf("Problema:\n");
                if(v == "NQUEENS"){
                    problema = 11;
                    printf("\tNQueens\n");
                }else if(v == "RADIOS"){
                    problema = 12;
                    printf("\tRadios\n");
                }else if(v == "NQUEENSPROFIT"){
                    problema = 13;
                    printf("\tNQueensProfit\n");
                }else if(v == "LABIRINTO"){
                    problema = 14;
                    printf("\tLabirinto\n");
                }else{
                    retorno += "Problema nao encontrado.\n";
                }
            }else if(k == "PLOT-INTERVAL"){
                printf("Plot-Interval:\n");
                intervalo_plot = stoi(v);
                printf("\t%d\n", intervalo_plot);
            }else if(k == "LIMITES"){
                string x;
                arquivo_parametros >> x;
                limite_inferior = stoi(v);
                limite_superior = stoi(x);
                printf("LIMITES: %d\t%d\n", limite_inferior, limite_superior);
            }else if(k == "ESCALONAMENTO"){
                escalonamento_linear = stoi(v);
                printf("Escalonamento linear:\n");
                printf("\t%d\n", escalonamento_linear);
            }
        }
        return retorno;
    }
};
#endif