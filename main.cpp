#include <bits/stdc++.h>
using namespace std;

#define BINARIO 0
#define INTEIRO 1
#define INTEIRO_PERMUTADO 2
#define REAL 3

class Dominio_Binario{
private:
    int tamanho_populacao;
    int tamanho_cromossomo;
    double limite_inferior, limite_superior;
    vector<vector<bool> > *individuos;
public:
    Dominio_Binario(int _tp, int _tc, double _li, double _ls, vector<vector<bool> > *_i){
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        individuos = _i;
        limite_inferior = _li;
        limite_superior = _ls;
        (*individuos).resize(tamanho_populacao, vector<bool>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        mt19937 engine(random_device{}());
        bernoulli_distribution distribution;

        for (int i = 0; i < tamanho_populacao; i++){
            for (int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = distribution(engine);
            }
        }
    }
      double fx(double x){
        return cos(20 * x) - (abs(x) / 2) + (pow(x, 3) / 4);
    }

    void fitnessCOS(){
        vector<pair<double, double> > fit;

        int l = pow(2, 16) - 1;
        double x, max, min;

        for(int i = 0; i < tamanho_populacao; i++){
            int d = 0;
            for(int j = tamanho_cromossomo - 1; j >= 0; j--){
                d += pow(2, j) * (*individuos)[i][j];
            }
            x = limite_inferior + (  ( (limite_superior - limite_inferior) * 1.0 )/ l) * d;
            max = (fx(x) + 4) / 6;
            min = 1 - (fx(x) + 4) / 6;
            fit.push_back(make_pair(max, min));
        }

        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\tmax: %.4lf\tmin: %.4lf\n", i, fit[i].first, fit[i].second);
        }
    }

    void fitnessFabricaRadios(){
        vector<pair<double, pair<double, double>>> fit;
        int valor_st, valor_lx;
        double penalizacao;
        double funcao_objetivo;

        for(size_t i = 0; i < tamanho_populacao; i++){
            valor_st = 0;
            valor_lx = 0;
            for(size_t j = 0; j < tamanho_cromossomo / 2; j++){
                valor_st += (*individuos)[i][j] * pow(2, j);
                valor_lx += (*individuos)[i][j + tamanho_cromossomo / 2] * pow(2, j);
            }
            valor_st = roundf(24.0 / 31 * valor_st);
            valor_lx = roundf(16.0 / 31 * valor_lx);
            penalizacao = max(0.0, ((valor_st + 2 * valor_lx - 40) * 1.0) / 16);
            funcao_objetivo = ((30 * valor_st + 40 * valor_lx) * 1.0) / 1360;
            fit.push_back(make_pair(funcao_objetivo - penalizacao, make_pair(funcao_objetivo, penalizacao)));
        }

        for(int i = 0; i < tamanho_populacao; i++){
            cout << i << ":\t" << "Fitness: " << fit[i].first << "\tFO: " << fit[i].second.first << "\tPEN: " << fit[i].second.second << endl;
        }
        cout << endl;
    }
};

class Dominio_Inteiro{
private:
    int tamanho_populacao;
    int tamanho_cromossomo;
    int limite_inferior, limite_superior;
    vector<vector<int> > *individuos;

public:
    Dominio_Inteiro(int _tp, int _tc, int _li, int _ls, vector<vector<int> > *_i){
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limite_inferior = _li;
        limite_superior = _ls;
        individuos = _i;
        (*individuos).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        random_device device;
        mt19937 engine(device());
        uniform_int_distribution<int> distribution(limite_inferior, limite_superior);

        for(int i = 0; i < tamanho_populacao; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = distribution(engine);
            }
        }
    }
    void fitness(){}
};

class Dominio_Inteiro_Permutado{
private:
    int tamanho_populacao;
    int tamanho_cromossomo;
    vector<vector<int>> *individuos;

  public:
    Dominio_Inteiro_Permutado(int _tp, int _tc, vector<vector<int>> *_i){
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        individuos = _i;
        (*individuos).resize(tamanho_populacao, vector<int>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        random_device device;
        mt19937 engine(device());

        for (int i = 0; i < tamanho_populacao; i++){
            for (int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = j;
            }
            shuffle((*individuos)[i].begin(), (*individuos)[i].end(), engine);
        }
    }

    bool fitness(int n){
        vector<pair<double, int> > fit;
        int colisoes;
        bool colidiu;

        for(int i = 0; i < tamanho_populacao; i++){ // percorre todas as soluções
            colisoes = 0;
            colidiu = false;
            for(int j = 0; j < tamanho_cromossomo; j++){ // percorre todas as rainha de uma solução
                colidiu = false;
                int y = (*individuos)[i][j];
                int x = j;
                int yc = y + 1;
                int yb = y - 1;
                for(int xx = x + 1; xx < tamanho_cromossomo; xx++){ // compara todas as rainhas com uma das rainhas
                    int q = (*individuos)[i][xx];
                    if(yc < tamanho_cromossomo){
                        if(yc == q){
                            colisoes++;
                            colidiu = true;
                            // cout << "x: " << x << " - y: " << y << "\txx: " << xx << " - yc: " << yc << endl;
                            break;
                        }
                    }
                    yc++;
                    if(yb >= 0){
                        if(yb == q){
                            colisoes++;
                            colidiu = true;
                            // cout << "x: " << x << " - y: " << y << "\txx: " << xx << " - yb: " << yb << endl;
                            break;
                        }
                    }
                    yb--;
                }
                if(colidiu) continue;
                yc = y + 1;
                yb = y - 1;
                for(int xx = x - 1; xx >= 0; xx--){ // compara todas as rainhas com uma das rainhas
                    int q = (*individuos)[i][xx];
                    if(yc < tamanho_cromossomo){
                        if(yc == q){
                            colisoes++;
                            colidiu = true;
                            // cout << "x: " << x << " - y: " << y << "\txx: " << xx << " - yc: " << yc << endl;
                            break;
                        }
                    }
                    yc++;
                    if(yb >= 0){
                        if(yb == q){
                            colisoes++;
                            colidiu = true;
                            // cout << "x: " << x << " - y: " << y << "\txx: " << xx << " - yb: " << yb << endl;
                            break;
                        }
                    }
                    yb--;
                }
            }

            double p = 1 - (colisoes / (1.0 * n));
            fit.push_back(make_pair(p, colisoes));
        }
        bool terminou = false;
        for(int i = 0; i < tamanho_populacao; i++){
            printf("%d.\t%.4lf\t%d\n", i, fit[i].first, fit[i].second);
        }
    }
};

class Dominio_Real{
private:
    int tamanho_populacao;
    int tamanho_cromossomo;
    double limite_inferior;
    double limite_superior;
    vector<vector<double> > *individuos;
public:
    Dominio_Real(int _tp, int _tc, double _li, double _ls, vector<vector<double> > *_i){
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limite_inferior = _li;
        limite_superior = _ls;
        individuos = _i;
        (*individuos).resize(tamanho_populacao, vector<double>(tamanho_cromossomo));
    }

    void gerar_populacao_inicial(){
        random_device device{};
        mt19937 engine{device()};
        uniform_real_distribution<double> distribution{limite_inferior, limite_superior};

        for(int i = 0; i < tamanho_populacao; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                (*individuos)[i][j] = distribution(engine);
            }
        }
    }
    void fitness(){}
  
};


class Populacao{
private:
    int tipo_variavel;
    int tamanho_populacao;
    int tamanho_cromossomo;
    double limite_inferior;
    double limite_superior;

    vector<vector<bool> > *individuos_binario = NULL;
    vector<vector<int> > *individuos_inteiro = NULL;
    vector<vector<int> > *individuos_inteiro_permutado = NULL;
    vector<vector<double> > *individuos_real = NULL;

    Dominio_Binario *db = NULL;
    Dominio_Inteiro_Permutado *dp = NULL;
    Dominio_Inteiro *di = NULL;
    Dominio_Real *dr = NULL;

public:
    Populacao(int _tv, int _tp, int _tc, double _li = 0, double _ls = 0){
        tipo_variavel = _tv;
        tamanho_populacao = _tp;
        tamanho_cromossomo = _tc;
        limite_inferior = _li;
        limite_superior = _ls;
        switch (tipo_variavel){
        case BINARIO:
            individuos_binario = new vector<vector<bool> >();
            db = new Dominio_Binario(tamanho_populacao, tamanho_cromossomo, limite_inferior, limite_superior, individuos_binario);
            break;
        case INTEIRO:
            individuos_inteiro = new vector<vector<int> >();
            di = new Dominio_Inteiro(tamanho_populacao, tamanho_cromossomo, limite_inferior, limite_superior, individuos_inteiro);
            break;
        case INTEIRO_PERMUTADO:
            individuos_inteiro_permutado = new vector<vector<int> >();
            dp = new Dominio_Inteiro_Permutado(tamanho_populacao, tamanho_cromossomo, individuos_inteiro_permutado);
            break;
        case REAL:
            individuos_real = new vector<vector<double> >();
            dr = new Dominio_Real(tamanho_populacao, tamanho_cromossomo, limite_inferior, limite_superior, individuos_real);
            break;
        }
    }

    void gerar_populacao_inicial(){
        switch(tipo_variavel){
            case BINARIO:
                (*db).gerar_populacao_inicial();
                break;
            case INTEIRO:
                (*di).gerar_populacao_inicial();
                break;
            case INTEIRO_PERMUTADO:
                (*dp).gerar_populacao_inicial();
                break;
            case REAL:
                (*dr).gerar_populacao_inicial();
                break;
        }
    }

    bool fitness(){
        switch(tipo_variavel){
            case BINARIO:
                (*db).fitnessFabricaRadios();
                break;
            case INTEIRO_PERMUTADO:
                (*dp).fitness(8);
                break;
        }
    }

    void print(){
        for (int i = 0; i < tamanho_populacao; i++){
            cout << i << "\t";
            for (int j = 0; j < tamanho_cromossomo; j++){
                if(j == tamanho_cromossomo / 2 ){
                    cout << "\t";
                }
                switch(tipo_variavel){
                    case BINARIO:
                        cout << (*individuos_binario)[i][j];
                        break;
                    case INTEIRO:
                        cout << (*individuos_inteiro)[i][j];
                        break;
                    case INTEIRO_PERMUTADO:
                        cout << (*individuos_inteiro_permutado)[i][j];
                        break;
                    case REAL:
                        cout << (*individuos_real)[i][j];
                        break;
                }
                cout << ' ';
            }
            cout << endl;
        }
    cout << endl;
    }
};

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int tipo_variavel, tamanho_populacao, tamanho_cromossomo;
    double limite_inferior, limite_superior;

    if(argc != 2){
        printf("%s <arquivo_parametros>\n", argv[0]);
        return 1;
    }

    ifstream arquivo_parametros;
    arquivo_parametros.open(argv[1]);

    arquivo_parametros >> tipo_variavel;
    arquivo_parametros >> tamanho_populacao;
    arquivo_parametros >> tamanho_cromossomo;
    arquivo_parametros >> limite_inferior;
    arquivo_parametros >> limite_superior;

    Populacao populacao(tipo_variavel, tamanho_populacao, tamanho_cromossomo, limite_inferior, limite_superior);
    
    bool t;

    populacao.gerar_populacao_inicial();
    populacao.print();
    populacao.fitness();


}
