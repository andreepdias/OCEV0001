#include <bits/stdc++.h>
#include "populacao.cpp"
using namespace std;

void Populacao::crossover_n_cortes_bin(int qtd_pontos_corte){
    map<int, bool> indices_ponto_corte;
    vector<int> cortes_ordenados;
    uniform_int_distribution<int> distribution{1, tamanho_cromossomo - 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        print_cromossomo(individuos_selecionados[i]);
        print_cromossomo(individuos_selecionados[i + 1]);
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
        print_cromossomo(individuos_selecionados[i]);
        print_cromossomo(individuos_selecionados[i + 1]);
        printf("\n");
    }
}

void Populacao::crossover_uniforme_bin(){

    uniform_int_distribution<int> distribution{0, 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        print_cromossomo(individuos_selecionados[i]);
        print_cromossomo(individuos_selecionados[i + 1]);
        for(int j = 0; j < tamanho_cromossomo; j++)
        {
            int moeda = distribution(engine);
            if(moeda){
                bool aux = (*individuos_binario)[individuos_selecionados[i]][j];
                (*individuos_binario)[individuos_selecionados[i]][j] = (*individuos_binario)[individuos_selecionados[i + 1]][j];
                (*individuos_binario)[individuos_selecionados[i + 1]][j] = aux;
            }
        }
        
        print_cromossomo(individuos_selecionados[i]);
        print_cromossomo(individuos_selecionados[i + 1]);
        printf("\n");
    }
}

void Populacao::crossover_pmx_intp(){
        uniform_int_distribution<int> distribution{1, tamanho_cromossomo - 1};

        vector<int> pontos_corte(2);
        map<int, int> matching_section_p1;
        map<int, int> matching_section_p2;

        for(int i = 0; i < tamanho_populacao; i += 2){
            matching_section_p1.clear();
            matching_section_p2.clear();

            pontos_corte[0] = distribution(engine);
            int r;
            do{
                r = distribution(engine);
            }while(r == pontos_corte[0]);
            pontos_corte[1] = r;

            sort(pontos_corte.begin(), pontos_corte.end());

            int p1 = individuos_selecionados[i];
            int p2 = individuos_selecionados[i + 1];
            print_cromossomo(p1);
            print_cromossomo(p2);
            printf("Cortes: ");
            for(int i = 0; i < 2; i++){
                printf("%d ", pontos_corte[i]);
            }
            printf("\n");
            for(int k = pontos_corte[0]; k < pontos_corte[1]; k++){

                matching_section_p1[(*individuos_inteiro_permutado)[p2][k]] = (*individuos_inteiro_permutado)[p1][k];
                matching_section_p2[(*individuos_inteiro_permutado)[p1][k]] = (*individuos_inteiro_permutado)[p2][k];

                int aux = (*individuos_inteiro_permutado)[p1][k];
                (*individuos_inteiro_permutado)[p1][k] = (*individuos_inteiro_permutado)[p2][k];
                (*individuos_inteiro_permutado)[p2][k] = aux;
            }

            for(int j = 0; j < tamanho_cromossomo; j++){

                int key = (*individuos_inteiro_permutado)[p1][j];
                if (matching_section_p1.find(key)  != matching_section_p1.end() ){
                    (*individuos_inteiro_permutado)[p1][j] = matching_section_p1[key];
                }
                key = (*individuos_inteiro_permutado)[p2][j];
                if (matching_section_p2.find(key)  != matching_section_p2.end() ){
                    (*individuos_inteiro_permutado)[p2][j] = matching_section_p2[key];
                }
            }
            print_cromossomo(p1);
            print_cromossomo(p2);
            printf("\n");
        }
}