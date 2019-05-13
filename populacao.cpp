#ifndef __POPULACAO_CPP
#define __POPULACAO_CPP

#include "dominios.cpp"
#include "parametros.cpp"
#include "relatorio.cpp"
using namespace std;


class Populacao{
public:
    int tipo_variavel;
    int tipo_selecao;
    int tipo_crossover;
    int tipo_mutacao;
    int tamanho_populacao;
    int tamanho_cromossomo;
    double probabilidade_crossover;
    double probabilidade_mutacao;
    bool elitismo;
    int problema;
    double *diversidade;

    int k; double kp;
    int t, d;
    
    int c;

    int draw;

    int execucoes;

    int intervalo_plot;

    vector<pair<double, double> > *limites;

    vector<vector<bool> > *individuos_binario = NULL;
    vector<vector<int> > *individuos_inteiro = NULL;
    vector<vector<int> > *individuos_inteiro_permutado = NULL;
    vector<vector<double> > *individuos_real = NULL;

    vector<vector<bool> > *individuos_intermediarios_binario = NULL;
    vector<vector<int> > *individuos_intermediarios_inteiro = NULL;
    vector<vector<int> > *individuos_intermediarios_inteiro_permutado = NULL;
    vector<vector<double> > *individuos_intermediarios_real = NULL;

    vector<bool> *melhor_individuo_binario = NULL;
    vector<int> *melhor_individuo_inteiro = NULL;
    vector<int> *melhor_individuo_inteiro_permutado = NULL;
    vector<double> *melhor_individuo_real = NULL;
    
    vector<int> *individuos_selecionados;

    vector<double> *infracoes;
    vector<double> *funcoes_objetivo;
    vector<double> *fitness;

    int melhor_individuo_indice;
    double melhor_individuo_fitness;
    double melhor_individuo_fo;
    double melhor_individuo_infracoes;

    Dominio_Binario *db = NULL;
    Dominio_Inteiro_Permutado *dp = NULL;
    Dominio_Inteiro *di = NULL;
    Dominio_Real *dr = NULL;

    vector<vector<bool> > *posicao_infracao;
    pair<double, vector<vector<double> > > *tabuleiro_lucro;

    random_device device{};
    mt19937 engine{device()};

public:
    Populacao(Parametros p){
        tipo_variavel = p.COD;
        tipo_selecao = p.COD_SELECTION;
        tipo_crossover = p.COD_CROSSOVER;
        tipo_mutacao = p.COD_MUTATION;

        tamanho_populacao = p.POP;
        tamanho_cromossomo = p.DIM;

        probabilidade_crossover = p.PC;
        probabilidade_mutacao = p.PM;
        diversidade = new double;
        (*diversidade) = 0;

        elitismo = p.ELIT;

        problema = p.PROBLEM;

        k = p.kk;
        kp = p.kp;
        t = p.t;
        cout << "   ---   T: " << t << endl;
        d = p.d;
        cout << "   ---   D: " << d << endl;
        c = p.c;
        
        draw = p.DRAW;

        execucoes = p.RUN;

        intervalo_plot = p.PLOT_INTERVAL;

        fitness = new vector<double>();
        (*fitness).resize(tamanho_populacao);
        
        individuos_selecionados = new vector<int>();
        (*individuos_selecionados).resize(tamanho_populacao);

        infracoes = new vector<double>();
        (*infracoes).resize(tamanho_populacao);

        funcoes_objetivo = new vector<double>();
        (*funcoes_objetivo).resize(tamanho_populacao);

        if(draw){
            posicao_infracao = new vector<vector<bool> >();
            (*posicao_infracao).resize(tamanho_populacao + 1, vector<bool>(tamanho_cromossomo));
        
        }

        if(problema == 3){
            tabuleiro_lucro = new pair<double, vector<vector<double> > >();
            (*tabuleiro_lucro).second.resize(tamanho_cromossomo, vector<double> (tamanho_cromossomo));
            double k = 0, kk;
            for(int i = 0; i < tamanho_cromossomo; i++){
                for(int j = 0; j < tamanho_cromossomo; j++){
                    k++;
                    if(i % 2 == 0){
                        kk = sqrt(k);
                    }else{
                        kk = log10(k);
                    }
                    (*tabuleiro_lucro).second[i][j] = kk;
                }
            }
            k = 0;
            for(int i = tamanho_cromossomo - 2, j = tamanho_cromossomo - 1; i >= 0 and j >= tamanho_cromossomo / 2; i -= 2, j--){
                k += (*tabuleiro_lucro).second[i][j];
            }
            for(int i = tamanho_cromossomo - 1, j = tamanho_cromossomo - 1; i >= 0 and j >= tamanho_cromossomo / 2; i -= 2, j--){
                k += (*tabuleiro_lucro).second[i][j];
            }
            (*tabuleiro_lucro).first = k;
            cout << (*tabuleiro_lucro).first << endl;
        }

        switch (tipo_variavel){
        case BINARIO:
            melhor_individuo_binario = new vector<bool>();
            individuos_binario = new vector<vector<bool> >();
            individuos_intermediarios_binario = new vector<vector<bool> >();
            db = new Dominio_Binario(tamanho_populacao, tamanho_cromossomo, limites, individuos_binario, individuos_intermediarios_binario, melhor_individuo_binario, fitness, individuos_selecionados, probabilidade_crossover, probabilidade_mutacao, infracoes, funcoes_objetivo, diversidade);
            break;
        case INTEIRO:
            melhor_individuo_inteiro = new vector<int>();
            individuos_inteiro = new vector<vector<int> >();
            individuos_intermediarios_inteiro = new vector<vector<int> >();
            di = new Dominio_Inteiro(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro, individuos_intermediarios_inteiro, melhor_individuo_inteiro, fitness, individuos_selecionados, probabilidade_crossover, probabilidade_mutacao, infracoes, funcoes_objetivo, diversidade);
            break;
        case INTEIRO_PERMUTADO:
            melhor_individuo_inteiro_permutado = new vector<int>();
            individuos_inteiro_permutado = new vector<vector<int> >();
            individuos_intermediarios_inteiro_permutado = new vector<vector<int> >();
            dp = new Dominio_Inteiro_Permutado(tamanho_populacao, tamanho_cromossomo, limites, individuos_inteiro_permutado, individuos_intermediarios_inteiro_permutado, melhor_individuo_inteiro_permutado, fitness, individuos_selecionados, probabilidade_crossover, probabilidade_mutacao, tabuleiro_lucro, infracoes, funcoes_objetivo, posicao_infracao, draw, diversidade);
            break;
        case REAL:
            melhor_individuo_real = new vector<double>();
            individuos_real = new vector<vector<double> >();
            individuos_intermediarios_real = new vector<vector<double> >();
            dr = new Dominio_Real(tamanho_populacao, tamanho_cromossomo, limites, individuos_real, individuos_intermediarios_real, melhor_individuo_real, fitness, individuos_selecionados, probabilidade_crossover, probabilidade_mutacao, infracoes, funcoes_objetivo, diversidade);
            break;
        }
    }

    void gerar_populacao_inicial(){
        switch(tipo_variavel){
            case BINARIO:
                (*db).gerar_populacao_inicial();
                (*melhor_individuo_binario) = (*individuos_binario)[0];
                break;
            case INTEIRO:
                (*di).gerar_populacao_inicial();
                (*melhor_individuo_inteiro) = (*individuos_inteiro)[0];
                break;
            case INTEIRO_PERMUTADO:
                (*dp).gerar_populacao_inicial();
                (*melhor_individuo_inteiro_permutado) = (*individuos_inteiro_permutado)[0];
                break;
            case REAL:
                (*dr).gerar_populacao_inicial();
                (*melhor_individuo_real) = (*individuos_real)[0];
                break;
        }
        melhor_individuo_fitness = 0;
        melhor_individuo_indice = 0;
        melhor_individuo_fo = 0;
        melhor_individuo_infracoes = 0;
    }

 
    void Fitness(int k, ofstream out[], int gen){
        if(problema == 1){
            (*dp).NQueens();
        }else if(problema == 2){
            (*db).radiosSTLX();
        }else if(problema == 3){
            (*dp).NQueensProfit();
        }

        double pior, melhor, media;
        int indice_melhor, indice_pior;
        pior = 1.1, melhor = -0.1, media = 0.0;
        for(int i = 0; i < tamanho_populacao; i++){

            double fit = (*fitness)[i];
            media += fit;

            if(fit >= melhor){
                indice_melhor = i;
                melhor = fit;
            }
            if(fit <= pior){
                indice_pior = i;
                pior = fit;
            }
        }
        media /= tamanho_populacao;
        
        if (draw){
            (*posicao_infracao)[tamanho_cromossomo] = (*posicao_infracao)[indice_melhor];
        }

        if(elitismo){
            /* TROCA PIOR ÍNDIVIDUO PELO MELHOR DA GERAÇÃO ANTERIOR */        
            switch(tipo_variavel){
                case BINARIO:
                    (*fitness)[indice_pior] = melhor_individuo_fitness;
                    (*individuos_binario)[indice_pior] = (*melhor_individuo_binario); break;
                case INTEIRO:
                    (*fitness)[indice_pior] = melhor_individuo_fitness;
                    (*individuos_inteiro)[indice_pior] = (*melhor_individuo_inteiro); break;
                case INTEIRO_PERMUTADO:
                    (*fitness)[indice_pior] = melhor_individuo_fitness;
                    (*individuos_inteiro_permutado)[indice_pior] = (*melhor_individuo_inteiro_permutado); break;
                case REAL:
                    (*fitness)[indice_pior] = melhor_individuo_fitness;
                    (*individuos_real)[indice_pior] = (*melhor_individuo_real); break;
            }
            /* MEMORIZA MELHOR INDIVIDUO DESSA GERAÇÃO */
            if(melhor >= melhor_individuo_fitness){
                melhor_individuo_fitness = melhor;
                melhor_individuo_fo = (*funcoes_objetivo)[indice_melhor];
                melhor_individuo_infracoes = (*infracoes)[indice_melhor];
                switch(tipo_variavel){
                    case BINARIO:
                        (*melhor_individuo_binario) = (*individuos_binario)[indice_melhor];
                        break;
                    case INTEIRO:
                        (*melhor_individuo_inteiro) = (*individuos_inteiro)[indice_melhor];
                        break;
                    case INTEIRO_PERMUTADO:
                        (*melhor_individuo_inteiro_permutado) = (*individuos_inteiro_permutado)[indice_melhor];
                        break;
                    case REAL:
                        (*melhor_individuo_real) = (*individuos_real)[indice_melhor];
                        break;
                }
            }else{
                indice_melhor = indice_pior;
            }
            melhor_individuo_indice = indice_melhor;


        }

        melhor = max(melhor, melhor_individuo_fitness);

        if((k + 1) % intervalo_plot == 0){
            out[gen] << k << " " << melhor << " " << pior << " " << media << endl;
            out[execucoes] << k << " " << melhor << " " << pior << " " << media << endl;
            
            
        }
    }

     void calcula_diversidade(int k, ofstream out[], int gen, double &maior_diversidade){
        if(problema == 1){
            (*dp).calcula_diversidade();
        }else if(problema == 2){
            // (*db).radiosSTLX();
        }else if(problema == 3){
            (*dp).calcula_diversidade();
        }
        maior_diversidade = max(maior_diversidade, (*diversidade));
        if((k + 1) % intervalo_plot == 0){
            out[gen] << k << " " << (*diversidade) << endl;
            out[execucoes] << k << " " << (*diversidade) << endl;
        }
     }

    void selecao(){
        switch(tipo_selecao){
            case 1: selecao_roleta(); break;
            case 2: selecao_ranking(); break;
            case 3: selecao_torneio(); break;
            case 4: selecao_vizinhanca(); break;
        }

        /* CRIA POPULAÇÃO INTERMEDIÁRIA */
        for(int i = 0; i < tamanho_populacao; i++){
            int s = (*individuos_selecionados)[i];
            switch(tipo_variavel){
                case BINARIO:
                    (*individuos_intermediarios_binario)[i] = (*individuos_binario)[s]; break;
                case INTEIRO:
                    (*individuos_intermediarios_inteiro)[i] = (*individuos_inteiro)[s]; break;
                   case INTEIRO_PERMUTADO:
                    (*individuos_intermediarios_inteiro_permutado)[i] = (*individuos_inteiro_permutado)[s]; break;
                case REAL:
                    (*individuos_intermediarios_real)[i] = (*individuos_real)[s]; break;
            }
        }

        /* ATUALIZA POPULAÇÃO PRINCIPAL */
        switch(tipo_variavel){
            case BINARIO:
                (*individuos_binario) = (*individuos_intermediarios_binario); break;
            case INTEIRO:
                (*individuos_inteiro) = (*individuos_intermediarios_inteiro); break;
            case INTEIRO_PERMUTADO:
                (*individuos_inteiro_permutado) = (*individuos_intermediarios_inteiro_permutado); break;
            case REAL:
                (*individuos_real) = (*individuos_intermediarios_real); break;
        }
    }

    void crossover(){
        switch(tipo_crossover){
        /*
            case 1:
                (*db).crossover_n_cortes_bin(c); break;
            case 2:
                (*db).crossover_n_cortes_int(c); break;
            case 3:
                (*di).crossover_uniforme_bin(c); break;
            case 4:
                (*di).crossover_uniforme_int(); break;
            case 5:
                (*dr).crossover_aritmetico_real(); break;
            case 6:
                (*dr).crossover_media_uniforme_real(); break;
                */
        case 8  :
            (*dp).crossover_pmx_intp();
        }
    }

    void mutation(){
        switch(tipo_mutacao){
            case 1:
                bit_flip(); break;
            case 3:
                (*dp).swap_mutation(); break;
        }
    }

    Relatorio relatorio_execucao(){
        vector <pair<string, double> >  variaveis;
        switch(problema){
            case 1:
                variaveis = (*dp).calcula_variaveis_nqueens(melhor_individuo_indice); break;
            case 2:
                variaveis = (*db).calcula_variaveis_radios(melhor_individuo_indice); break;
            case 3:
                variaveis = (*dp).calcula_variaveis_nqueens(melhor_individuo_indice); break;
        }
        Relatorio r(
            (*fitness)[melhor_individuo_indice], 
            (*funcoes_objetivo)[melhor_individuo_indice], 
            (*infracoes)[melhor_individuo_indice], 
            variaveis);
        return r;
    }
    void print_populacao(){
        printf("Populacao:\n");
        for (int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t", i);
            for (int j = 0; j < tamanho_cromossomo; j++){
                switch(tipo_variavel){
                    case BINARIO:
                        cout << (*individuos_binario)[i][j]; break;
                    case INTEIRO:
                        cout << (*individuos_inteiro)[i][j]; break;
                    case INTEIRO_PERMUTADO:
                        cout << (*individuos_inteiro_permutado)[i][j]; break;
                    case REAL:
                        cout << (*individuos_real)[i][j]; break;
                }
                printf(" ");
            }
            printf("\t(%lf)", (*fitness)[i]);
            printf("\n");
        }
        printf("\n");
    }
    void print_cromossomo(int g){
        printf("%d.\t", g);
        for (int j = 0; j < tamanho_cromossomo; j++){
            switch(tipo_variavel){
                case BINARIO:
                    cout << (*individuos_binario)[g][j]; break;
                case INTEIRO:
                    cout << (*individuos_inteiro)[g][j]; break;
                case INTEIRO_PERMUTADO:
                    cout << (*individuos_inteiro_permutado)[g][j]; break;
                case REAL:
                    cout << (*individuos_real)[g][j]; break;
            }
            printf(" ");
        }
        printf("\n");
    }
    void print_fitness(){
        printf("Fitness:\n");
        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t%.4lf\n", i, (*fitness)[i]);
        }
        printf("\n");
    }
    void print_selecionados(){
        printf("Selecao:\n");
        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t%d\t(%.10lf)\n", i, (*individuos_selecionados)[i], (*fitness)[(*individuos_selecionados)[i]]);
        }
        printf("\n");
    }


    int girar_roleta(vector<pair<double, int> > &participantes, int ignorado);
    void roleta(vector<int> &individuos, function<double(int)> valor, int n_escolhidos, int &k);

    void selecao_roleta();
    void selecao_ranking();
    void selecao_torneio();
    void selecao_vizinhanca();

    void bit_flip();

    // void crossover_n_cortes_bin(int qtd_pontos_corte);
    // void crossover_uniforme_bin();
    

};

#endif
   /*
    void drawQueens(sf::RenderWindow &window, int indice){
        sf::RectangleShape blank(sf::Vector2f(WINDOW_SIZE, WINDOW_SIZE));
        sf::RectangleShape item(sf::Vector2f(WINDOW_SIZE / (double) tamanho_cromossomo, WINDOW_SIZE / (double) tamanho_cromossomo));
        sf::RectangleShape lh (sf::Vector2f(WINDOW_SIZE * tamanho_cromossomo, 1));
        sf::RectangleShape lv(sf::Vector2f(1, WINDOW_SIZE));
        
        blank.setFillColor(sf::Color(255, 255, 255));
        blank.setPosition(0, 0);
        window.draw(blank);

        for(int i = 0; i < tamanho_cromossomo; i++){
            int y = i;
            int x = (*melhor_individuo_inteiro_permutado)[i];

            if((*posicao_infracao)[tamanho_cromossomo][i]){
                item.setFillColor(sf::Color(255, 0, 0));
            }else{
                item.setFillColor(sf::Color(0, 255, 0));
            }

            item.setPosition(WINDOW_SIZE * x / (double)tamanho_cromossomo, WINDOW_SIZE * y / (double) tamanho_cromossomo);
            window.draw(item);

            lv.setFillColor(sf::Color(175, 175, 175));
            lv.setPosition(WINDOW_SIZE * i / (double)tamanho_cromossomo, 0);
            window.draw(lv);

            lh.setFillColor(sf::Color(175, 175, 175));
            lh.setPosition(0, WINDOW_SIZE * i / (double)tamanho_cromossomo);
            window.draw(lh);
        }
        window.display();
    }
    */

    // void Fitness(int k, ofstream out[], int gen, sf::RenderWindow &window){

        /*
            if(draw){
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }
                drawQueens(window, indice_melhor);
            }
            */