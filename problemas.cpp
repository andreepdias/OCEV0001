#ifndef __PROBLEMAS_CPP
#define __PROBLEMAS_CPP

#include "dominios.cpp"
using namespace std;


void Dominio_Binario::funcaoCOS(){

    double x, fx, max, min;
    int l = pow(2, 16) - 1, d;

    for(int i = 0; i < tamanho_populacao; i++){
        d = 0;
        for(int j = tamanho_cromossomo - 1; j >= 0; j--){
            d += pow(2, j) * (*individuos)[i][j];
        }
        x = (*limites)[0].first + (  ( (*limites)[0].second - (*limites)[0].first )/ l) * d;
        fx = cos(20 * x) - (abs(x) / 2) + (pow(x, 3) / 4);
        max = (fx + 4) / 6;
        min = 1 - (fx + 4) / 6;
        (*fitness)[i] = (max);
    }

}

void Dominio_Binario::radiosSTLX()
{
    double valor_st, valor_lx;
    double valor_funcao_objetivo, penalizacao;

    for(size_t i = 0; i < tamanho_populacao; i++){
        valor_st = 0;
        valor_lx = 0;
        for(size_t j = 0; j < tamanho_cromossomo / 2; j++){
            valor_st += (*individuos)[i][j] * pow(2, j);
            valor_lx += (*individuos)[i][j + tamanho_cromossomo / 2] * pow(2, j);
        }
        valor_st = roundf((24.0 / 31) * valor_st);
        valor_lx = roundf((16.0 / 31) * valor_lx);
        penalizacao = max(0.0, double(valor_st + 2 * valor_lx - 40) / 16); //penalização baseada na quantidade de homens possível
        (*infracoes)[i] = penalizacao;
        valor_funcao_objetivo = ((30 * valor_st + 40 * valor_lx) * 1.0);
        (*funcoes_objetivo)[i] = valor_funcao_objetivo;
        valor_funcao_objetivo /= 1040;  //normalização com o total alcançavel
        (*fitness)[i] = max((valor_funcao_objetivo - penalizacao),0.0);
    }
    
}

vector<pair<string, double> > Dominio_Binario::calcula_variaveis_radios(int indice ){
    vector<pair<string, double> > variaveis(2);
    double st = 0, lx = 0;
    for (int i = 0; i < tamanho_cromossomo / 2; i++){
        st += (*individuos)[indice][i] * pow(2, i);
        lx += (*individuos)[indice][i + tamanho_cromossomo / 2] * pow(2, i);
    }
    variaveis[0] = make_pair("standard", st);
    variaveis[1] = make_pair("deluxe", lx);
    return variaveis;
}

void Dominio_Inteiro_Permutado::NQueens()
{

    #pragma omp parallel 
    #pragma omp for schedule(dynamic)
    for(int i = 0; i < tamanho_populacao; i++){ // percorre todas as soluções
        int colisoes = 0;
        double fo = 0;
        for(int j = 0; j < tamanho_cromossomo; j++){ // percorre todas as rainha de uma solução
            int y = (*individuos)[i][j];
            int x = j;
            int yc = y + 1;
            int yb = y - 1;
            bool colidiu = false;
            for(int xx1 = x + 1, xx2 = x - 1; xx1 < tamanho_cromossomo or xx2 >= 0; xx1++, xx2--){ // compara todas as rainhas com uma das rainhas
                int q1, q2;

                if (xx1 < tamanho_cromossomo)
                {
                    q1 = (*individuos)[i][xx1];
                }else{
                    q1 = -1;
                }
                
                if(xx2 >= 0){
                    q2 = (*individuos)[i][xx2];
                }else{
                    q2 = -1;
                }

                if(yc < tamanho_cromossomo){
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
            if(!colidiu){
                 if(draw) (*posicao_infracao)[i][j] = false; 
            }else{
                 if(draw) (*posicao_infracao)[i][j] = true;
            }
        }
        // (*fitness)[i] = (1 - (colisoes * 1.0) / tamanho_cromossomo);
        fo = tamanho_cromossomo - colisoes;
        (*funcoes_objetivo)[i] = fo;
        (*infracoes)[i] = colisoes;
        (*fitness)[i] = fo / tamanho_cromossomo;
    }
}

void Dominio_Inteiro_Permutado::NQueensProfit()
{

    #pragma omp parallel 
    #pragma omp for schedule(dynamic)
    for(int i = 0; i < tamanho_populacao; i++){ // percorre todas as soluções
        int colisoes = 0;
        double fo = 0;
        for(int j = 0; j < tamanho_cromossomo; j++){ // percorre todas as rainha de uma solução
            int y = (*individuos)[i][j];
            int x = j;
            int yc = y + 1;
            int yb = y - 1;
            bool colidiu = false;
            for(int xx1 = x + 1, xx2 = x - 1; xx1 < tamanho_cromossomo or xx2 >= 0; xx1++, xx2--){ // compara todas as rainhas com uma das rainhas
                int q1, q2;

                if (xx1 < tamanho_cromossomo)
                {
                    q1 = (*individuos)[i][xx1];
                }else{
                    q1 = -1;
                }
                
                if(xx2 >= 0){
                    q2 = (*individuos)[i][xx2];
                }else{
                    q2 = -1;
                }

                if(yc < tamanho_cromossomo){
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
            if(!colidiu){
                fo += double((*tabuleiro_lucro).second[j][y]);
                 if(draw) (*posicao_infracao)[i][j] = false; 
            }else{
                 if(draw) (*posicao_infracao)[i][j] = true;
            }
        }
        (*funcoes_objetivo)[i] = fo;
        (*infracoes)[i] = colisoes;
        (*fitness)[i] = fo / (*tabuleiro_lucro).first;
    }
}


vector <pair<string, double> >  Dominio_Inteiro_Permutado::calcula_variaveis_nqueens(int indice ){
    vector <pair<string, double> > variaveis(tamanho_cromossomo);

    for(int i = 0; i < tamanho_cromossomo; i++){
        variaveis[i].second = double((*individuos)[indice][i] + 1);
        variaveis[i].first = to_string(i + 1);
    }
    return variaveis;
}

bool isValid(int x, int y, int sx, int sy){
    if(x < 0 or y < 0 or x >= sx or y >= sy)
        return false;
    return true;
}

double Dominio_Inteiro::fx_infracao(int j){
    double fx = 1.0 / sqrt(j);
    fx = fx > 1.0 ? 1 : fx;
    return fx;
}

void Dominio_Inteiro::labirinto(){

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    int sx = matrix_labirinto.size();
    int sy = matrix_labirinto[0].size();


    for(int i = 0;  i< tamanho_populacao; i++){ // percorre todas as soluções
        int celulas_validas = 0, celulas_invalidas = 0, celulas_diferentes = 0;
        int x = 10, y = 1;
        bool celula_final = false;
        int numero_movimentos = tamanho_cromossomo;

        vector<vector<bool> > visitados(sx, vector<bool> (sy, false));
        visitados[x][y] = true;

        double fitness_movimentos = 1.0;
        pair<int, int> anterior = make_pair(10, 1);

        for(int j = 0; j < tamanho_cromossomo; j++){ // percorre todas as rainha de uma solução
            int k = (*individuos)[i][j];

            int c = 0, aux_k = 0;
            while(matrix_labirinto[x + dx[k]][y + dy[k]] == 0){
                k = (k + 1) % 4;
                if (matrix_labirinto[x + dx[k]][y + dy[k]] != 0){
                    if (!visitados[x + dx[k]][y + dy[k]]){
                        break;                        
                    }else{
                        aux_k = k;
                    }
                }
                c++;
                if(c == 4){
                    k = aux_k;
                    break;
                }
            }

            if(matrix_labirinto[x + dx[k]][y + dy[k]] != 0 and !visitados[x + dx[k]][y + dy[k]]){
                celulas_validas++;

                x = x + dx[k];
                y = y + dy[k];

                if(!visitados[x][y]){
                    visitados[x][y] = true;
                    celulas_diferentes++;
                }
            }
            
            if(matrix_labirinto[x][y] == 3){
                celula_final = true;
                numero_movimentos = j + 1;
                break;
            }
        }
        double distancia = 1 - x + 20  - y;
        celulas_invalidas = numero_movimentos - celulas_validas;

        (*funcoes_objetivo)[i] = celulas_diferentes;
        (*infracoes)[i] = celulas_invalidas;
        (*fitness)[i] = (celulas_diferentes / (numero_movimentos + pow(celulas_invalidas, 2)));
        //  * 0.6 + (1.0 - (distancia / 55.0)) * 0.4;
    }

}


vector<pair<string, double>> Dominio_Inteiro::calcula_variaveis_labirinto(int indice)
{
    vector<vector<int> > matrix = {
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
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            matrix[i][j] = 0;
        }
    }

    vector<pair<string, double>> variaveis(2);

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    int sx = matrix_labirinto.size();
    int sy = matrix_labirinto[0].size();

    int x = 10, y = 1;
    matrix[x][y] = 1;
    vector<vector<bool>> visitados(sx, vector<bool>(sy, false));
    visitados[x][y] = true;
    for(int j = 0; j < tamanho_cromossomo; j++){ // percorre todas as rainha de uma solução
            int k = (*individuos)[indice][j];

            int c = 0, aux_k = 0;
            while(matrix_labirinto[x + dx[k]][y + dy[k]] == 0){
                k = (k + 1) % 4;
                if (matrix_labirinto[x + dx[k]][y + dy[k]] != 0){
                    if (!visitados[x + dx[k]][y + dy[k]]){
                        break;                        
                    }else{
                        aux_k = k;
                    }
                }
                c++;
                if(c == 4){
                    k = aux_k;
                    break;
                }
            }

            if(matrix_labirinto[x + dx[k]][y + dy[k]] != 0 and !visitados[x + dx[k]][y + dy[k]]){

                x = x + dx[k];
                y = y + dy[k];
                matrix[x][y] = 1;

                if(!visitados[x][y]){
                    visitados[x][y] = true;
                }
            }
            
            if(matrix_labirinto[x][y] == 3){
                break;
            }
        }

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    variaveis[0] = make_pair("x", x);
    variaveis[1] = make_pair("y", y);

    return variaveis;
}

#endif


// correção
// mutação dos pontos de colisão


// topologico, situacional, temporal