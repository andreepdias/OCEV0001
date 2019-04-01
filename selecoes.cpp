#include <bits/stdc++.h>
#include "populacao.cpp"
using namespace std;

void Populacao::selecao_roleta(void *parametros)
{
    random_device device{};
    mt19937 engine{device()};
    uniform_real_distribution<double> distribution{0.0, 1.0};

    vector<double> fitness_relativo(tamanho_populacao);
    double somatorio, somatorio_atual, soma_acumulada, r;

    somatorio = 0.0;
    for (int i = 0; i < tamanho_populacao; i++){
        somatorio += (*fitness)[i];
    }

    int k = 0, individuo_selecionado = -1;
    for (int x = 0; x < tamanho_populacao; x++){

        somatorio_atual = somatorio;
        if (individuo_selecionado != -1){
            somatorio_atual -= (*fitness)[individuo_selecionado];
        }
        somatorio_atual = somatorio_atual == 0 ? 1 : somatorio_atual;

        for (int i = 0; i < tamanho_populacao; i++){
            if (i == individuo_selecionado) continue;
            fitness_relativo[i] = (*fitness)[i] / somatorio_atual;
        }

        soma_acumulada = 0.0;
        r = distribution(engine);

        for (int i = 0; i < tamanho_populacao; i++){
            if (i == individuo_selecionado) continue;
            if (fitness_relativo[i] + soma_acumulada >= r){
                individuos_selecionados[k++] = i;
                individuo_selecionado = i;
                break;
            }
            soma_acumulada += fitness_relativo[i];
        }
    }
}

void Populacao::selecao_ranking(void *parametros)
{
    random_device device{};
    mt19937 engine{device()};
    uniform_real_distribution<double> distribution{0.0, 1.0};

    vector<pair<double, int>> ranking_individuos(tamanho_populacao);
    vector<double> fitness_relativo(tamanho_populacao);

    for (int i = 0; i < tamanho_populacao; i++){
        ranking_individuos[i] = make_pair((*fitness)[i], i);
    }
    sort(ranking_individuos.begin(), ranking_individuos.end());

    double somatorio = double(tamanho_populacao * (tamanho_populacao + 1)) / 2;
    for (int i = 0; i < tamanho_populacao; i++){
        fitness_relativo[ranking_individuos[i].second] = double(i + 1) / somatorio;
        // printf("Raking %i:\ti:%d\tfit:%lf\n", i + 1, ranking_individuos[i].second, ranking_individuos[i].first);
    }

    double soma_acumulada, r;
    int k = 0;
    for (int x = 0; x < tamanho_populacao; x++){
        soma_acumulada = 0.0;
        r = distribution(engine);

        for (int i = 0; i < tamanho_populacao; i++){
            if (fitness_relativo[i] + soma_acumulada >= r){
                individuos_selecionados[k++] = i;
                break;
            }
            soma_acumulada += fitness_relativo[i];
        }
    }
}

void Populacao::selecao_torneio(void *parametros)
{
    random_device device{};
    mt19937 engine{device()};
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
        // printf("Torneio %d:\n", x + 1);
        for (int i = 0; i < k; i++){
            do{
                r = distribution_int(engine);
            }while(individuos_escolhidos.find(r) != individuos_escolhidos.end());

            individuos_escolhidos[r] = true;
            individuos_torneio[i].first = (*fitness)[r];
            individuos_torneio[i].second = r;
        }
        sort(individuos_torneio.begin(), individuos_torneio.end());

        // printf("\nIndividuos Torneio:\n");
        // for(int i = 0; i < k; i++){
        // printf("i: %d\tfit: %lf\n", individuos_torneio[i].second, individuos_torneio[i].first);
        // }

        rr = distribution_real(engine);

        if (kp >= rr){
            individuos_selecionados[x] = individuos_torneio[k - 1].second;
        }else{
            individuos_selecionados[x] = individuos_torneio[0].second;
        }
        // printf("\nIndividuo Escolhido: %d\n\n___\n\n", individuos_selecionados[x]);
    }
}

void Populacao::selecao_vizinhanca(void *parametros)
{
    random_device device{};
    mt19937 engine{device()};
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

    pair<double, int> melhor_individuo;


    for (int x = 0; x < tamanho_populacao / 2; x++){
        individuo_aleatorio = distribution(engine);
        individuos_selecionados[x * 2] = individuo_aleatorio;

        printf("Individuo escolhido:\t\t\t\t%d\n", individuo_aleatorio);
        if(t == 1){ // ESCOLHE MELHOR INDIVÍDUO DA VIZINHANÇA
            melhor_individuo.first = 0.0, melhor_individuo.second = -1;
            for (int i = -d; i <= d; i++){
                if (i == 0) continue;

                int p = individuo_aleatorio + i;
                if (p < 0){
                    p = p + tamanho_populacao;
                }else{
                    p = p % tamanho_populacao;
                }

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
                // printf("r: %d\n", r);
            }while(r == 0);
            int p = r + individuo_aleatorio;
            if (p < 0){
                p = p + tamanho_populacao;
            }else{
                p = p % tamanho_populacao;
            }
            individuos_selecionados[x * 2 + 1] = p;
            printf("Inviduo escolhido aleatorio vizinhanca:\t\t%d\n\n", p);
        }else if(t == 3){ // ESCOLHE INDIVÍDUO DA VIZINHANÇA
            double somatorio = 0;
            int individuo_escolhido;
            for(i = -d; i <= d; i++){
                if(i == 0) continue;
                int p = individuo_aleatorio + i;
                if (p < 0){
                    p = p + tamanho_populacao;
                }else{
                    p = p % tamanho_populacao;
                }
                somatorio += (*fitness)[p];
            }
            printf("Somatorio: %lf\n", somatorio);
            if(somatorio == 0){
                do{
                    r = distribution_d(engine);
                    // printf("r: %d\n", r);
                }while (r == 0);
                int p = r + individuo_aleatorio;
                if (p < 0){
                    p = p + tamanho_populacao;
                }else{
                    p = p % tamanho_populacao;
                }
                individuo_escolhido = p;
            }else{
                vector<pair<double, int> > fitness_relativo(d * 2 + 1);
                for(i = -d; i <= d; i++){
                    if(i == 0) continue;

                    int p = individuo_aleatorio + i;
                    if (p < 0){
                        p = p + tamanho_populacao;
                    }else{
                        p = p % tamanho_populacao;
                    }
                    fitness_relativo[i + d].first = (*fitness)[p] / somatorio;
                    fitness_relativo[i + d].second = p;
                    printf("Individuo Aleatorio: %d\tIndividuo Vizinho: %d\ti:%d\td:%di+d:%d\n", individuo_aleatorio, p, i, d, i + d);
                }
                printf("Fitness Relativo:\n");
                for(i = 0; i < d * 2 + 1; i++){
                    printf("i:%d\tp:%d\tfit:%lf\n", i, fitness_relativo[i].second, fitness_relativo[i].first);
                }
                double r = distribution_real(engine);
                double soma_acumulada = 0;
                individuo_escolhido = -1;
                for(i = -d; i <= d; i++){
                    if(i == 0) continue;


                    if(fitness_relativo[i + d].first + soma_acumulada >= r){
                        individuo_escolhido = fitness_relativo[i + d].second;
                        break;
                    }
                    soma_acumulada += fitness_relativo[i + d].first;
                }

            }
            individuos_selecionados[x * 2 + 1] = individuo_escolhido;
            printf("Inviduo escolhido proporcinal vizinhanca:\t%d\n\n", individuo_escolhido);
        }
    }
    if(tamanho_populacao % 2 != 0){
        individuos_selecionados[tamanho_populacao - 1] = distribution(engine);
    }
}
