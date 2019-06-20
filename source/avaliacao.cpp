#ifndef __AVALIACAO_CPP
#define __AVALIACAO_CPP
using namespace std;

#include "populacao.cpp"

class Avaliacao {
public:
    Parametros *p;    
    Populacao *populacao;

    pair<int, double> melhor_individuo;
    pair<int, double> pior_individuo;
    pair<int, double> segundo_pior_individuo;

    double fitness_medio;

    void *dados_problema;

    Avaliacao(){}    
    Avaliacao(Parametros *_p, Populacao *_populacao){
        p = _p;
        populacao = _populacao;

        carregar_dados_problema();
    }

    void avaliacao(){
        switch((*p).problema){
            case RAINHAS:       NQueens(); break;
            case RAINHAS_LUCRO: break;
            case RADIOS: break;
            case LABIRINTO:     labirinto(); break;
            case MICHALEWICZ:   michaelewicz(); break;
            case KEANES:        keanes(); break;
        }

        melhor_individuo.first = 0;
        melhor_individuo.second = (*populacao).individuos[0].fitness;

        pior_individuo.first = 0;
        pior_individuo.second = (*populacao).individuos[0].fitness;
        
        segundo_pior_individuo.first = 0;
        segundo_pior_individuo.second = (*populacao).individuos[0].fitness;

        fitness_medio = 0;

        for(int i = 1; i < (*p).tamanho_populacao; i++){
            double fitness = (*populacao).individuos[i].fitness;
            if(fitness > melhor_individuo.second){
                melhor_individuo.first = i;
                melhor_individuo.second = fitness;
            }
            if(fitness <= pior_individuo.second){

                segundo_pior_individuo.first = pior_individuo.first;
                segundo_pior_individuo.second = pior_individuo.second;

                pior_individuo.first = i;
                pior_individuo.second = fitness;

            }else if(fitness < segundo_pior_individuo.second){
                segundo_pior_individuo.first = i;
                segundo_pior_individuo.second = fitness;
            }
            fitness_medio += fitness;
        }
        fitness_medio /= (*p).tamanho_populacao;
    }

    void NQueens()
    {
        #pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < (*p).tamanho_populacao; i++){ 
            vector<int> *c = (vector<int>*)(*populacao).individuos[i].cromossomo;
            int colisoes = 0;
            double fo = 0;
            for(int j = 0; j < (*p).tamanho_cromossomo; j++){
                int y = (*c)[j];
                int x = j;
                int yc = y + 1;
                int yb = y - 1;
                bool colidiu = false;
                for(int xx1 = x + 1, xx2 = x - 1; xx1 < (*p).tamanho_cromossomo or xx2 >= 0; xx1++, xx2--){ // compara todas as rainhas com uma das rainhas
                    int q1, q2;

                    if (xx1 < (*p).tamanho_cromossomo)
                    {
                        q1 = (*c)[xx1];
                    }else{
                        q1 = -1;
                    }
                    
                    if(xx2 >= 0){
                        q2 = (*c)[xx2];
                    }else{
                        q2 = -1;
                    }

                    if(yc < (*p).tamanho_cromossomo){
                        if(yc == q1 or yc == q2){
                            colisoes++;
                            colidiu = true;
                            break;
                        }
                    }
                    if(yb >= 0){
                        if(yb == q1 or yb == q2){
                            colisoes++;
                            colidiu = true;
                            break;
                        }
                    }
                    yc++;
                    yb--;
                }
            }
            fo = (*p).tamanho_cromossomo - colisoes;
            (*populacao).individuos[i].funcao_objetivo = fo;
            (*populacao).individuos[i].infracao = colisoes;
            (*populacao).individuos[i].fitness = fo / (*p).tamanho_cromossomo;
        }
    }

    void labirinto()
    {

        if((*p).fix){
            fix_individuos_labirinto();
        }

        vector<vector<int> > *matrix_labirinto = (vector<vector<int> >*)dados_problema;

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        int sx = (*matrix_labirinto).size();
        int sy = (*matrix_labirinto)[0].size();

        #pragma omp parallel for schedule(dynamic)
        for(int i = 0;  i< (*p).tamanho_populacao; i++){ 
            int celulas_validas = 0, celulas_invalidas = 0, celulas_diferentes = 0, celulas_repetidas = 0;
            int numero_movimentos = (*p).tamanho_cromossomo;
            double distancia = 0;
            bool celula_final = false;
            
            int x = 10, y = 1;

            vector<vector<bool> > visitados(sx, vector<bool>(sy, false));
            visitados[x][y] = true;
            
            vector<int> *c = (vector<int>*)(*populacao).individuos[i].cromossomo;
            vector<vector<int> > *d = (vector<vector<int>>*)(*populacao).individuos[i].dados_individuo;

            for(int a = 0; a < sx; a++){
                for(int b = 0; b < sy; b++){
                    (*d)[a][b] = 0;
                }
            }
            
            for(int j = 0; j < (*p).tamanho_cromossomo; j++){
                
                int k = (*c)[j];

                if((*matrix_labirinto)[x + dx[k]][y + dy[k]] != 0){
                    celulas_validas++;

                    x = x + dx[k];
                    y = y + dy[k];
                    (*d)[x][y] = 1;
               
                    if(!visitados[x][y]){
                        visitados[x][y] = true;
                        celulas_diferentes++;
                    }
                }else{
                    celulas_invalidas++;
                }
                
                
                if((*matrix_labirinto)[x][y] == 3){
                    celula_final = true;
                    numero_movimentos = j + 1;
                    break;
                }
                
            }
            distancia = abs(x - 1) + abs(y - 20);

            // double fitness = (double(celulas_validas) / (numero_movimentos)) * (1.0 - (double(distancia) / 55)) * (double(celulas_diferentes) / numero_movimentos);
            double fitness = (1.0 - (double(distancia) / 55)) * (double(celulas_diferentes) / numero_movimentos) * (1.0 + (1.0 - (double(numero_movimentos) / (*p).tamanho_cromossomo)));

            (*populacao).individuos[i].funcao_objetivo = celulas_validas;
            (*populacao).individuos[i].infracao = celulas_invalidas;
            (*populacao).individuos[i].fitness = fitness;

            (*d)[29][0] = celulas_validas;
            (*d)[29][1] = celulas_invalidas;
            (*d)[29][2] = celulas_diferentes;
            (*d)[29][3] = numero_movimentos;
            (*d)[29][4] = distancia;
            (*d)[29][5] = celulas_repetidas;
            (*d)[29][6] = (*populacao).individuos[i].fitness * 100000;
        }
    }

    void fix_individuos_labirinto()
    {
        
        mt19937 engine(random_device{}());
        uniform_int_distribution<int> distribution((*p).limite_inferior, (*p).limite_superior);

        vector<vector<int> > *matrix_labirinto = (vector<vector<int> >*)dados_problema;

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        int sx = (*matrix_labirinto).size();
        int sy = (*matrix_labirinto)[0].size();

        #pragma omp parallel for schedule(dynamic)
        for(int i = 0;  i < (*p).tamanho_populacao; i++){ 
            
            int x = 10, y = 1;
            vector<int> *c = (vector<int>*)(*populacao).individuos[i].cromossomo;
            vector<int> fix_c((*p).tamanho_cromossomo, -1);
            
            int a = 0;
            for(int j = 0; j < (*p).tamanho_cromossomo; j++){
                int k = (*c)[j];

                if((*matrix_labirinto)[x + dx[k]][y + dy[k]] != 0){
                    x = x + dx[k];
                    y = y + dy[k];
                    fix_c[a++] = k;
                }
            }
            for(int j = 0; j < (*p).tamanho_cromossomo; j++){
                (*c)[j] = fix_c[j];
                if(fix_c[j] == -1){
                    (*c)[j] = distribution(engine);
                }
            }
        }
    }

    void carregar_dados_problema()
    {
        switch((*p).problema){
            case LABIRINTO:
                dados_problema = (void*)new vector<vector<int> > (30, vector<int> (25));
                montar_labirinto();
                break;
        }
    }

    void montar_labirinto()
    {

        vector<vector<int> > *matrix_labirinto = (vector<vector<int> >*)dados_problema;

        (*matrix_labirinto) = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 3, 1, 1, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
            {0, 2, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0},
            {0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0},
            {0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
            {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
            {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
            {0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
            {0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
    }

    void print_melhor_individiduo()
    {
        switch((*p).problema){
            case LABIRINTO:
                print_melhor_individuo_labirinto();
                break;
            case KEANES:
                print_melhor_individuo_keanes();
                break;
        }
    }

    void print_melhor_individuo_labirinto()
    {
        
        vector<vector<int> > *matrix_labirinto = (vector<vector<int> >*)dados_problema;

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        int sx = (*matrix_labirinto).size();
        int sy = (*matrix_labirinto)[0].size();

        vector<vector<int> > *d = (vector<vector<int>>*)(*populacao).melhor_individuo.dados_individuo;

        printf("fitness: %.5lf   validas: %d    invalidas: %d    diferentes: %d    movimentos: %d    distancia: %d\n", 
                (*populacao).melhor_individuo.fitness, 
                (*d)[29][0], 
                (*d)[29][1], 
                (*d)[29][2], 
                (*d)[29][3], 
                (*d)[29][4]);

        for(int i = 0; i < sx; i ++){
            for(int j = 0; j < sy; j++){

                if((*matrix_labirinto)[i][j] == 0){
                    printf(". ");
                }else if((*matrix_labirinto)[i][j] == 2 or (*matrix_labirinto)[i][j] == 3){
                    printf("O ");
                }else if((*d)[i][j] == 1){
                    printf("# ");
                }else{
                    printf("  ");
                }
            }
            printf("\n");
        }
    }

    void print_melhor_individuo_keanes()
    {
        vector<double> *d = (vector<double>*)(*populacao).melhor_individuo.dados_individuo;

        printf("fitness: %.5lf   f: %.5lf    r1: %.5lf(%lf)    r2: %.5lf(%.5lf)\n", 
                (*populacao).melhor_individuo.fitness, 
                (*d)[0], 
                (*d)[1], 
                (*d)[3], 
                (*d)[2], 
                (*d)[4]);
    }

    void michaelewicz(){

        #pragma omp parallel for schedule(dynamic)
        for(int x = 0; x < (*p).tamanho_populacao; x++){

            double fitness = 0;
            vector<double> *c = (vector<double>*)(*populacao).individuos[x].cromossomo;
            

            for(int i = 0; i < (*p).tamanho_cromossomo; i++){
                fitness += sin((*c)[i]) * pow(sin(((i + 1) * pow((*c)[i], 2)) / M_PI), 2 * 10);
            }
            (*populacao).individuos[x].fitness = fitness;
        }
    }

    void keanes(){

        #pragma omp parallel for schedule(dynamic)
        for(int k = 0; k < (*p).tamanho_populacao; k++){

            vector<double> *cromossomo = (vector<double>*)(*populacao).individuos[k].cromossomo;
            vector<double> *dados_individuo = (vector<double>*)(*populacao).individuos[k].dados_individuo;

            double a = 0.0, b = 1.0, c = 0.0, x;

            for(int i = 0; i < (*p).tamanho_cromossomo; i++){

                (*cromossomo)[i] = max(0.0000000001, (*cromossomo)[i]);
                (*cromossomo)[i] = min(9.9999999999, (*cromossomo)[i]);

                x = (*cromossomo)[i];
                a += pow(cos(x), 4);
            }
            for(int i = 0; i < (*p).tamanho_cromossomo; i++){
                x = (*cromossomo)[i];
                b *= pow(cos(x), 2);
            }
            b *= 2;

            for(int i = 0; i < (*p).tamanho_cromossomo; i++){
                x = (*cromossomo)[i];
                c += (i + 1) * pow(x, 2);
            }
            c = sqrt(c);
            if(c == 0) c = DBL_EPSILON;

            double f = abs(a - b) / c;

            double r1 = 1.0;
            for(int i = 0; i < (*p).tamanho_cromossomo; i++){
                x = (*cromossomo)[i];
                r1 *= x;
            }

            double r2 = 0.0;
            for(int i = 0; i < (*p).tamanho_cromossomo; i++){
                r2 += x;
            }

            double pr1 = 1.0, pr2 = 1.0;
            if(r1 <= 0.75){
                pr1 = (r1 / (0.75 + 0.0000000001));
                f = f * pr1;
            }
            
            double max = (*p).tamanho_cromossomo * (*p).limite_superior;
            double m = ((15.0 * double((*p).tamanho_cromossomo)) / 2.0) - 0.0000000001;
            if(r2 >= max){
                pr2 = (1.0 - ((r2 - m) / (max - m)));
                f = f * pr2;
            }

            (*populacao).individuos[k].fitness = f;
            
            (*dados_individuo)[0] = f;
            (*dados_individuo)[1] = r1;
            (*dados_individuo)[2] = r2;
            (*dados_individuo)[3] = pr1;
            (*dados_individuo)[4] = pr2;
        }

    }


};

#endif