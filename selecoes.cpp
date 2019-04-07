#include <bits/stdc++.h>
#include "populacao.cpp"
using namespace std;

int Populacao::girar_roleta(vector<pair<double, int> > &participantes, int ignorado){
    uniform_real_distribution<double> distribution{0.0, 1.0};
    
    double soma_acumulada = 0.0;
    double r = distribution(engine);

    for(int i = 0; i < participantes.size(); i++){
        if(participantes[i].second == ignorado) continue;
        if(soma_acumulada + participantes[i].first >= r){
            return participantes[i].second;
        }
        soma_acumulada += participantes[i].first;
    }
}

void Populacao::roleta(vector<int> &individuos, function<double(int)> valor, int n_escolhidos, int &k)
{
    vector<pair<double, int> > fitness_relativo(individuos.size());

    double somatorio = 0.0;
    for(int i = 0; i < individuos.size(); i++){ // CALCULA A SOMA DOS FITNESS
        somatorio += valor(individuos[i]);
    }

    double somatorio_atual, individuo_escolhido = -1;
    while(n_escolhidos--){ // ESCOLHE OS INDIVIUDOS 
        somatorio_atual = somatorio;

        if(individuo_escolhido != -1){ // IGONRA INDIVIUDO ESCOLHIO
            somatorio_atual -= valor(individuo_escolhido);
        }
        somatorio_atual = somatorio_atual == 0 ? DBL_EPSILON : somatorio_atual;
    
        for(int x = 0; x < individuos.size(); x++){ // CALCULA FINESS RELATIVO
            if(individuos[x] == individuo_escolhido) continue;
            fitness_relativo[x].second = individuos[x];
            fitness_relativo[x].first = valor(individuos[x]) / somatorio_atual;
        }
        int escolhido = girar_roleta(fitness_relativo, individuo_escolhido); // GIRA ROLETA E ESCOLHE
        individuos_selecionados[k++] = individuo_escolhido = escolhido;
    }
}

void Populacao::selecao_roleta(void *parametros)
{
    int k = 0;
    vector<int> individuos(tamanho_populacao);
    for(int i = 0; i < tamanho_populacao; i++){
        individuos[i] = i;
    }

    auto valor = [&](int individuo) { return (*fitness)[individuo]; };
    roleta(individuos, valor, tamanho_populacao, k);
}

void Populacao::selecao_ranking(void *parametros)
{
    vector<pair<double, int>> ranking_individuos(tamanho_populacao);

    for (int i = 0; i < tamanho_populacao; i++){
        ranking_individuos[i] = make_pair((*fitness)[i], i);
    }
    sort(ranking_individuos.begin(), ranking_individuos.end());

    int k = 0;
    vector<int> individuos(tamanho_populacao);         // INDICES RANKING ELEMENTO INDIVIDUO
    vector<int> individuos_ranking(tamanho_populacao); // INDICES INDIVIDUO ELEMENTO RANKING
    for(int i = 0; i < tamanho_populacao; i++){
        individuos[i] = ranking_individuos[i].second;
        individuos_ranking[individuos[i]] = i + 1;
    }

    auto valor = [&](int individuo) { return individuos_ranking[individuo]; };
    roleta(individuos, valor, tamanho_populacao, k);
}

void Populacao::selecao_torneio(void *parametros)
{
    uniform_real_distribution<double> distribution_real{0.0, 1.0};
    uniform_int_distribution<int> distribution_int(0, tamanho_populacao - 1);

    pair<int, double> *p = (pair<int, double> *)parametros;

    int k = (*p).first;
    double kp = (*p).second;

    vector<pair<double, int>> individuos_torneio(k);
    map<int, bool> individuos_escolhidos;

    int r;
    double rr;

    for (int x = 0; x < tamanho_populacao; x++){
        individuos_escolhidos.clear();
        for (int i = 0; i < k; i++){
            do{
                r = distribution_int(engine);
            }while(individuos_escolhidos.find(r) != individuos_escolhidos.end());

            individuos_escolhidos[r] = true;
            individuos_torneio[i].first = (*fitness)[r];
            individuos_torneio[i].second = r;
        }
        sort(individuos_torneio.begin(), individuos_torneio.end());

        rr = distribution_real(engine);

        if (kp >= rr){
            individuos_selecionados[x] = individuos_torneio[k - 1].second;
        }else{
            individuos_selecionados[x] = individuos_torneio[0].second;
        }
    }
}

void Populacao::selecao_vizinhanca(void *parametros)
{
    uniform_int_distribution<int> distribution(0, tamanho_populacao - 1);

    pair<int, int> *p = (pair<int, int>*)parametros;
    int d = (*p).first;
    int t = (*p).second;

    int r, i;
    int individuo_aleatorio;

    uniform_int_distribution<int> distribution_d(-d, d);
    uniform_real_distribution<double> distribution_real{0.0, 1.0};

    if (d >= tamanho_populacao){
        d = tamanho_populacao - 1;
    }

    auto posicao = [&](int individuo_aleatorio, int x){
        int p = individuo_aleatorio + x;
        return (p < 0) ? (p + tamanho_populacao) : (p % tamanho_populacao);
    };

    for(int x = 0; x < tamanho_populacao / 2; x++){
        individuo_aleatorio = distribution(engine);
        individuos_selecionados[x * 2] = individuo_aleatorio;

        printf("Individuo escolhido:\t\t\t\t%d\n", individuo_aleatorio);
        if(t == 1){ // ESCOLHE MELHOR INDIVÍDUO DA VIZINHANÇA
            pair<double, int> melhor_individuo = make_pair(0.0, -1);
            for (int i = -d; i <= d; i++){
                if (i == 0) continue;

                int p = posicao(individuo_aleatorio, i);

                if ((*fitness)[p] >= melhor_individuo.first){
                    melhor_individuo.first = (*fitness)[p];
                    melhor_individuo.second = p;
                }
            }
            individuos_selecionados[x * 2 + 1] = melhor_individuo.second;
            printf("Inviduo escolhido melhor vizinhanca:\t\t%d\n\n", melhor_individuo.second);
        }else if(t == 2){ // ESCOLHE INDIVÍDUO ALEATÓRIO DA VIZINHANÇA
            do{
                r = distribution_d(engine);
            }while(r == 0);
            
            int p = posicao(individuo_aleatorio, r);

            individuos_selecionados[x * 2 + 1] = p;
            printf("Inviduo escolhido aleatorio vizinhanca:\t\t%d\n\n", p);
        }else if(t == 3){ // ESCOLHE INDIVÍDUO DA VIZINHANÇA PROPORCINAL AO FITNESS
            double somatorio = 0;

            vector<int> individuos(d * 2);

            int k = 0;
            for(int i = -d; i <= d; i++){
                if(i == 0) continue;
                individuos[k++] = posicao(individuo_aleatorio, i);
            }
            auto valor = [&](int individuo) { return (*fitness)[individuo]; };
            k = x *  2 + 1;
            roleta(individuos, valor, 1, k);
        }
    }
    if(tamanho_populacao % 2 != 0){
        individuos_selecionados[tamanho_populacao - 1] = distribution(engine);
    }
}
