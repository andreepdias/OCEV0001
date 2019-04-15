#ifndef __CROSSOVER_CPP
#define __CROSSOVER_CPP

#include "populacao.cpp"
using namespace std;

void Dominio_Binario::crossover_n_cortes_bin(int qtd_pontos_corte){
    map<int, bool> indices_ponto_corte;
    vector<int> cortes_ordenados;
    uniform_int_distribution<int> distribution{1, tamanho_cromossomo - 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        /*
        print_cromossomo((*individuos_selecionados)[i]);
        print_cromossomo((*individuos_selecionados)[i + 1]);
        */
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

        /*
        printf("Cortes: ");
        for(int i = 0; i < qtd_pontos_corte; i++){
            printf("%d ", cortes_ordenados[i]);
        }
        printf("\n");
        */
        for(int j = 0; j < qtd_pontos_corte; j += 2)
        {
            for(int k = cortes_ordenados[j]; k < cortes_ordenados[j + 1]; k++)
            {
                bool aux = (*individuos)[(*individuos_selecionados)[i]][k];
                (*individuos)[(*individuos_selecionados)[i]][k] = (*individuos)[(*individuos_selecionados)[i + 1]][k];
                (*individuos)[(*individuos_selecionados)[i + 1]][k] = aux;
            }
        }
        /*
        print_cromossomo((*individuos_selecionados)[i]);
        print_cromossomo((*individuos_selecionados)[i + 1]);
        printf("\n");
        */
    }
}

void Dominio_Binario::crossover_uniforme_bin()
{

    uniform_int_distribution<int> distribution{0, 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        /*
        print_cromossomo((*individuos_selecionados)[i]);
        print_cromossomo((*individuos_selecionados)[i + 1]);
        */
        for(int j = 0; j < tamanho_cromossomo; j++)
        {
            int moeda = distribution(engine);
            if(moeda){
                bool aux = (*individuos)[(*individuos_selecionados)[i]][j];
                (*individuos)[(*individuos_selecionados)[i]][j] = (*individuos)[(*individuos_selecionados)[i + 1]][j];
                (*individuos)[(*individuos_selecionados)[i + 1]][j] = aux;
            }
        }
        /*
        print_cromossomo((*individuos_selecionados)[i]);
        print_cromossomo((*individuos_selecionados)[i + 1]);
        printf("\n");
        */
    }
}

/*
void Populacao::crossover_n_cortes_int(int qtd_pontos_corte){
    map<int, bool> indices_ponto_corte;
    vector<int> cortes_ordenados;
    uniform_int_distribution<int> distribution{1, tamanho_cromossomo - 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        print_gene((*individuos_selecionados)[i]);
        print_gene((*individuos_selecionados)[i + 1]);
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
                int aux = (*individuos_inteiro)[(*individuos_selecionados)[i]][k];
                (*individuos_inteiro)[(*individuos_selecionados)[i]][k] = (*individuos_inteiro)[(*individuos_selecionados)[i + 1]][k];
                (*individuos_inteiro)[(*individuos_selecionados)[i + 1]][k] = aux;
            }
        }
        print_gene((*individuos_selecionados)[i]);
        print_gene((*individuos_selecionados)[i + 1]);
        printf("\n");
    }
}

void Populacao::crossover_uniforme_int(){

    uniform_int_distribution<int> distribution{0, 1};

    for(int i = 0; i < tamanho_populacao; i += 2)
    {
        print_gene((*individuos_selecionados)[i]);
        print_gene((*individuos_selecionados)[i + 1]);
        for(int j = 0; j < tamanho_cromossomo; j++)
        {
            int moeda = distribution(engine);
            if(moeda){
                int aux = (*individuos_inteiro)[(*individuos_selecionados)[i]][j];
                (*individuos_inteiro)[(*individuos_selecionados)[i]][j] = (*individuos_inteiro)[(*individuos_selecionados)[i + 1]][j];
                (*individuos_inteiro)[(*individuos_selecionados)[i + 1]][j] = aux;
            }
        }
        
        print_gene((*individuos_selecionados)[i]);
        print_gene((*individuos_selecionados)[i + 1]);
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
*/
void Dominio_Inteiro_Permutado::crossover_pmx_intp()
{
    uniform_int_distribution<int> distribution{1, tamanho_cromossomo - 1};

    vector<int> pontos_corte(2);
    map<int, int> matching_section_p1;
    map<int, int> matching_section_p2;

    for (int i = 0; i < tamanho_populacao; i += 2)
    {
        matching_section_p1.clear();
        matching_section_p2.clear();

        pontos_corte[0] = distribution(engine);
        int r;
        do
        {
            r = distribution(engine);
        } while (r == pontos_corte[0]);
        pontos_corte[1] = r;

        sort(pontos_corte.begin(), pontos_corte.end());

        int p1 = (*individuos_selecionados)[i];
        int p2 = (*individuos_selecionados)[i + 1];
        /*
            print_cromossomo(p1);
            print_cromossomo(p2);
            printf("Cortes: ");
            for(int i = 0; i < 2; i++){
                printf("%d ", pontos_corte[i]);
            }
            printf("\n");
            */
        for (int k = pontos_corte[0]; k < pontos_corte[1]; k++)
        {

            int aux = (*individuos)[p1][k];
            (*individuos)[p1][k] = (*individuos)[p2][k];
            (*individuos)[p2][k] = aux;

            int e1 = (*individuos)[p1][k];
            int e2 = (*individuos)[p2][k];

            matching_section_p1[e1] = e2;
            matching_section_p2[e2] = e1;
        }

        for (int j = 0; j < pontos_corte[0]; j++)
        {
            int key = (*individuos)[p1][j];
            while (matching_section_p1.find(key) != matching_section_p1.end())
            {
                key = matching_section_p1[key];
            }
            (*individuos)[p1][j] = key;

            key = (*individuos)[p2][j];
            while (matching_section_p2.find(key) != matching_section_p2.end())
            {
                key = matching_section_p2[key];
            }
            (*individuos)[p2][j] = key;
        }
        for (int j = pontos_corte[1]; j < tamanho_cromossomo; j++)
        {
            int key = (*individuos)[p1][j];
            while (matching_section_p1.find(key) != matching_section_p1.end())
            {
                key = matching_section_p1[key];
            }
            (*individuos)[p1][j] = key;

            key = (*individuos)[p2][j];
            while (matching_section_p2.find(key) != matching_section_p2.end())
            {
                key = matching_section_p2[key];
            }
            (*individuos)[p2][j] = key;
        }
        /*
            print_cromossomo(p1);
            print_cromossomo(p2);
            printf("\n");
            */
    }
}

#endif