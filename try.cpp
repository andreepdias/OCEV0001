#include <bits/stdc++.h>
using namespace std;

int main()
{
    int tamanho_cromossomo = 10;


    vector<int> pontos_corte(2);
    pontos_corte[0] = 3;
    pontos_corte[1] = 8;

    vector<int> parent2 = {8, 4, 7, 3, 6, 2, 5, 1, 9, 0};
    vector<int> parent1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    vector<int> child1(tamanho_cromossomo);

    vector<pair<int, int> > notInChild;

    map<int, int> posParent1;
    map<int, int> posParent2;

    map<int, int> change;

    for (int k = pontos_corte[0]; k < pontos_corte[1]; k++)
    {
        child1[k] = parent1[k];

        posParent1[parent1[k]] = k;
        posParent2[parent2[k]] = k;
    }

    for (int k = pontos_corte[0]; k < pontos_corte[1]; k++){
        if(posParent1.find(parent2[k]) == posParent1.end()){
            notInChild.push_back(make_pair(parent2[k], k));
        }
    }
    for(int k = 0; k < notInChild.size(); k++){
        int value = notInChild[k].first;
        int index = notInChild[k].second;

        int key = parent1[index];

        while(posParent2.find(key) != posParent2.end()){
            int i = posParent2[key];
            key = parent1[i];
        }

        change[key] = value;
    }

    for(int i = 0; i < pontos_corte[0]; i++){
        
        if(change.find(parent2[i]) == change.end()){
            child1[i] = parent2[i];
        }else{
            child1[i] = change[parent2[i]];
        }
    }
    for(int i = pontos_corte[1]; i < tamanho_cromossomo; i++){
        
        if(change.find(parent2[i]) == change.end()){
            child1[i] = parent2[i];
        }else{
            child1[i] = change[parent2[i]];
        }
    }



    auto print = [&](vector<int> &p) {
        for (int i = 0; i < tamanho_cromossomo; i++)
        {
            printf("%d\t", p[i]);
        }
        printf("\n");
    };

    print(child1);
    /*
        print_cromossomo(p1);
        print_cromossomo(p2);
        printf("\n");
    */
}