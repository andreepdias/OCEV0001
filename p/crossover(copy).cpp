#include <bits/stdc++.h>
#include "populacao.cpp"
using namespace std;

void Populacao::crossover_n_cortes_bin(int qtd_pontos_corte){
    map<int, bool> indices_ponto_corte;
    vector<int> cortes_ordenados;
    uniform_int_distribution<int> distribution{1, tamanho_cromossomo - 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        print_gene(individuos_selecionados[i]);
        print_gene(individuos_selecionados[i + 1]);
        for(int j = 0; j < qtd_pontos_corte; j++)
        {
            int ponto_corte_aleatorio;
            do
            {
                ponto_corte_aleatorio = distribution(engine);

            } while (indices_ponto_corte.find(ponto_corte_aleatorio) != indices_ponto_corte.end());
            indices_ponto_corte[ponto_corte_aleatorio] = true;
            cortes_ordenados.push_back(ponto_corte_aleatorio);
        }        
        cortes_ordenados.push_back(tamanho_cromossomo);
        sort(cortes_ordenados.begin(), cortes_ordenados.end());

        printf("Cortes: ");
        for(int i = 0; i < qtd_pontos_corte; i++){
            printf("%d ", cortes_ordenados[i]);
        }
        printf("\n");

        for(int j = 0; j < qtd_pontos_corte; j += 2)
        {
            for(int k = cortes_ordenados[j]; k < cortes_ordenados[j + 1]; k++)
            {
                bool aux = (*individuos_binario)[individuos_selecionados[i]][k];
                (*individuos_binario)[individuos_selecionados[i]][k] = (*individuos_binario)[individuos_selecionados[i + 1]][k];
                (*individuos_binario)[individuos_selecionados[i + 1]][k] = aux;
            }
        }
        print_gene(individuos_selecionados[i]);
        print_gene(individuos_selecionados[i + 1]);
        printf("\n");
    }
}

void Populacao::crossover_uniforme_bin(){

    uniform_int_distribution<int> distribution{0, 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        print_gene(individuos_selecionados[i]);
        print_gene(individuos_selecionados[i + 1]);
        for(int j = 0; j < tamanho_cromossomo; j++)
        {
            int moeda = distribution(engine);
            if(moeda){
                bool aux = (*individuos_binario)[individuos_selecionados[i]][j];
                (*individuos_binario)[individuos_selecionados[i]][j] = (*individuos_binario)[individuos_selecionados[i + 1]][j];
                (*individuos_binario)[individuos_selecionados[i + 1]][j] = aux;
            }
        }
        
        print_gene(individuos_selecionados[i]);
        print_gene(individuos_selecionados[i + 1]);
        printf("\n");
    }
}

void Populacao::crossover_n_cortes_int(int qtd_pontos_corte){
    map<int, bool> indices_ponto_corte;
    vector<int> cortes_ordenados;
    uniform_int_distribution<int> distribution{1, tamanho_cromossomo - 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        print_gene(individuos_selecionados[i]);
        print_gene(individuos_selecionados[i + 1]);
        for(int j = 0; j < qtd_pontos_corte; j++)
        {
            int ponto_corte_aleatorio;
            do
            {
                ponto_corte_aleatorio = distribution(engine);

            } while (indices_ponto_corte.find(ponto_corte_aleatorio) != indices_ponto_corte.end());
            indices_ponto_corte[ponto_corte_aleatorio] = true;
            cortes_ordenados.push_back(ponto_corte_aleatorio);
        }        
        cortes_ordenados.push_back(tamanho_cromossomo);
        sort(cortes_ordenados.begin(), cortes_ordenados.end());

        printf("Cortes: ");
        for(int i = 0; i < qtd_pontos_corte; i++){
            printf("%d ", cortes_ordenados[i]);
        }
        printf("\n");

        for(int j = 0; j < qtd_pontos_corte; j += 2)
        {
            for(int k = cortes_ordenados[j]; k < cortes_ordenados[j + 1]; k++)
            {
                int aux = (*individuos_inteiro)[individuos_selecionados[i]][k];
                (*individuos_inteiro)[individuos_selecionados[i]][k] = (*individuos_inteiro)[individuos_selecionados[i + 1]][k];
                (*individuos_inteiro)[individuos_selecionados[i + 1]][k] = aux;
            }
        }
        print_gene(individuos_selecionados[i]);
        print_gene(individuos_selecionados[i + 1]);
        printf("\n");
    }
}

void Populacao::crossover_uniforme_int(){

    uniform_int_distribution<int> distribution{0, 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        print_gene(individuos_selecionados[i]);
        print_gene(individuos_selecionados[i + 1]);
        for(int j = 0; j < tamanho_cromossomo; j++)
        {
            int moeda = distribution(engine);
            if(moeda){
                int aux = (*individuos_inteiro)[individuos_selecionados[i]][j];
                (*individuos_inteiro)[individuos_selecionados[i]][j] = (*individuos_inteiro)[individuos_selecionados[i + 1]][j];
                (*individuos_inteiro)[individuos_selecionados[i + 1]][j] = aux;
            }
        }
        
        print_gene(individuos_selecionados[i]);
        print_gene(individuos_selecionados[i + 1]);
        printf("\n");
    }
}

void Populacao::crossover_blx_real(double alpha = 0.5){
    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        for(int j = 0; j < tamanho_cromossomo; j++)
        {
            double maior_i = max(individuos_real[i][j], individuos_real[i + 1][j]);
            double menor_i = min(individuos_real[i][j], individuos_real[i + 1][j]);
            double d = maior_i - menor_i;
            maior_i += d*alpha;
            menor_i -= d*alpha;
            uniform_int_distribution<double> distribution{menor_i, maior_i};
            individuos_real[i][j] = distribution(engine);
            individuos_real[i + 1][j] = distribution(engine);
        }
    }
}

void Populacao::crossover_media_uniforme_real(){

    uniform_int_distribution<int> distribution{0, 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        for(int j = 0; j < tamanho_cromossomo; j++)
        {
            double media = (individuos_real[i][j] + individuos_real[i + 1][j])/2;
            int moeda = distribution(engine);
            if(moeda){
                individuos_real[i][j] = media;
            }else{
                individuos_real[i + 1][j] = media;
            }
        }
    }
}

void Populacao::crossover_aritmetico_real(double alpha = 0.5){
    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        for(int j = 0; j < tamanho_cromossomo; j++)
        {
            double ind_temp_1 = alpha * individuos_real[i][j] + (1 - alpha) * individuos_real[i + 1][j];
            double ind_temp_2 = (1 - alpha) * individuos_real[i][j] + alpha * individuos_real[i + 1][j];
            individuos_real[i][j] = ind_temp_1;
            individuos_real[i + 1][j] = ind_temp_2;
        }
    }
}