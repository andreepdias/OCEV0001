/*
indivudos
50 ~ 100

crossover
*/

// -lsfml-graphics -lsfml-window -lsfml-system
#ifndef __MAIN_CPP
#define __MAIN_CPP

#include "populacao.cpp"

using namespace std;

void relatorio_fim_execucoes(Parametros &parametros, vector<Relatorio> &relatorios);
void grafico_convergencia(Parametros &parametros, double &maior_diversidade);
void print_relatorio(Relatorio &r, int g, int gg);

int main(int argc, char const *argv[])
{

    if (argc < 2)
    {
        printf("%s <arquivo_parametros>\n", argv[0]);
    }

    Parametros parametros;
    string r = parametros.carregar_parametros(argv[1]);
    if (r.size() > 0)
    {
        cout << r << endl;
        return 1;
    }
    /*
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "NQueens"); 
    if(!parametros.DRAW){
        window.close();
    }
    */

    ofstream graficos[parametros.RUN + 1];
    ofstream diversidade[parametros.RUN + 1];

    vector<Relatorio> relatorios(parametros.RUN);
    Populacao populacao(parametros);

    double maior_diversidade = 0;

    for (int k = 0; k < parametros.RUN; k++)
    {
        string s = "tempos_" + to_string(k + 1);
        graficos[k].open(s.c_str());
        graficos[parametros.RUN].open("tempos");

        // s = "diversidade_" + to_string(k + 1);
        // diversidade[k].open(s.c_str());
        // diversidade[parametros.RUN].open("diversidade");

        populacao.gerar_populacao_inicial();
        for (int i = 0; i < parametros.GEN; i++)
        {
            populacao.Fitness(k, graficos, i);
            // populacao.print_populacao();
    //         populacao.calcula_diversidade(i, diversidade, k, maior_diversidade);
            populacao.selecao();
            populacao.crossover();
            // populacao.mutation();
        }
        populacao.Fitness(parametros.GEN, graficos, k);
    //     populacao.calcula_diversidade(parametros.GEN, diversidade, k, maior_diversidade);

        relatorios[k] = populacao.relatorio_execucao();

        print_relatorio(relatorios[k], k + 1, parametros.RUN);

        graficos[k].close();
        graficos[parametros.RUN].close();
    }

    // grafico_convergencia(parametros, maior_diversidade);
    relatorio_fim_execucoes(parametros, relatorios);

    /*
if(parametros.DRAW){
    bool pressed = false;
    while(pressed == false){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            pressed = true;
        }
    }
    window.close();
}
*/
}

void print_relatorio(Relatorio & r, int g, int gg)
{
    printf("Execucao: %2d/%2d --- Funcao Objetivo: %lf --- Infracoes: %lf --- x: %.0lf --- y: %.0lf\n", g, gg, r.funcao_objetivo, r.infracao_restricoes, r.variaveis[0].second, r.variaveis[1].second);
}

void relatorio_fim_execucoes(Parametros & parametros, vector<Relatorio> & relatorios)
{
    double media = 0;
    double desvio = 0;
    double melhor_fo = -1.0;
    int indice_melhor_fo = -1;
    for (int i = 0; i < parametros.RUN; i++)
    {
        // printf("Execucao %d:\n", i + 1);
        // printf("\tMelhor solucao encontada:\n");
        // printf("\t\tFuncao Objetivo: %lf\n", relatorios[i].funcao_objetivo);
        // printf("\t\tInfracoes: %lf\n", relatorios[i].infracao_restricoes);
        media += relatorios[i].funcao_objetivo;

        if (relatorios[i].funcao_objetivo > melhor_fo)
        {
            indice_melhor_fo = i;
            melhor_fo = relatorios[i].funcao_objetivo;
        }
    }
    media /= parametros.RUN;

    for (int i = 0; i < parametros.RUN; i++)
    {
        desvio += pow(media - relatorios[i].funcao_objetivo, 2);
    }
    desvio /= parametros.RUN;
    desvio = sqrt(desvio);

    printf("\nFinal das execucoes:\n");
    printf("\tFuncao Objetivo:\n");
    printf("\t\tMedia:\t%lf\n", media);
    printf("\t\tDesvio Padrao:\t%lf\n", desvio);
    printf("\n\tMelhor solucao: execucao %d\n", indice_melhor_fo + 1);
    printf("\n\t\tFuncao Objetivo: %lf\n", relatorios[indice_melhor_fo].funcao_objetivo);
    printf("\t\tInfracoes: %lf\n", relatorios[indice_melhor_fo].infracao_restricoes);
    printf("\n\t\tVariaveis:\n");
    for (int i = 0; i < relatorios[indice_melhor_fo].variaveis.size(); i++)
    {
        cout << relatorios[indice_melhor_fo].variaveis[i].first << ":";
        printf("%4.0lf --- ", relatorios[indice_melhor_fo].variaveis[i].second);
    }
    cout << endl;
}

void grafico_convergencia(Parametros & parametros, double &maior_diversidade)
{

    ifstream in[parametros.RUN];
    ifstream ind[parametros.RUN];
    ofstream graficos;
    ofstream diversidades;

    for (int i = 0; i < parametros.RUN; i++)
    {
        string s = "tempos_" + to_string(i + 1);
        in[i].open(s.c_str());
        s = "diversidade_" + to_string(i + 1);
        ind[i].open(s.c_str());
    }
    graficos.open("tempos");
    diversidades.open("diversidade");

    int geracao;
    double melhor, pior, media;
    double m_melhor, m_pior, m_media;
    double diversidade, m_diversidade;

    for (int i = 0; i < parametros.GEN / parametros.PLOT_INTERVAL; i++)
    {
        m_melhor = 0.0, m_pior = 0.0, m_media = 0.0, diversidade = 0.0;
        for (int k = 0; k < parametros.RUN; k++)
        {
            in[k] >> geracao >> melhor >> pior >> media;
            ind[k] >> diversidade;
            m_melhor += melhor;
            m_pior += pior;
            m_media += media;
        }
        m_melhor /= parametros.RUN;
        m_pior /= parametros.RUN;
        m_media /= parametros.RUN;
        m_diversidade /= parametros.RUN / maior_diversidade;
        graficos << geracao << " " << m_melhor << " " << m_pior << " " << m_media << endl;
        diversidades << geracao << " " << m_diversidade << endl;
    }
}

#endif
