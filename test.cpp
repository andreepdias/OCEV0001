#include <bits/stdc++.h>
using namespace std;

typedef vector<double> vb;

bool fmin(vb &fitness, double avg, double c){
    int size = fitness.size();

    if(fitness[0] > ((c * avg) - fitness[size - 1]) / (c - 1)){
        return true;
    }
    return false;
}

int main(){

    double c = 2;

    vb fitness(4);
    double f;

    double soma = 0;
    for(int i = 0; i < 4; i++){
        cin >> f;
        fitness[i] = f;
        soma += f;
    }
    sort(fitness.begin(), fitness.end());
    for(int i = 0; i < 4; i++){
        double r = fitness[i] / soma;
        printf("Fitness[%lf]: %lf\n", fitness[i], r);
    }
    printf("\n");

    int size = fitness.size();
    double avg = 0;
    for(int i = 0; i < size; i++){
        avg += fitness[i];
    }
    avg /= size;

    double alpha = 0, beta = 0;
    if(fmin(fitness, avg, c)){
        alpha = (avg * (c - 1))  / (fitness[size - 1] - avg);
        beta = (avg * (fitness[size - 1] - (c * avg))) / (fitness[size - 1] - avg);
    }else{
        alpha = avg / (avg - fitness[0]);
        beta = (-fitness[0] * avg) / (avg - fitness[0]);
    }
    printf("alpha: %lf\tbeta: %lf\n\n", alpha, beta);

    double sum = 0;
    for(int i = 0; i < 4; i++){
        double e = (alpha * fitness[i]) + beta;
        sum += e;
        printf("Escalonado[%lf]: %lf\n", fitness[i], e);
    }
    printf("\n");
    printf("Sum: %lf\n\n", sum);

    for(int i = 0; i < 4; i++){
        double e = (alpha * fitness[i]) + beta;
        e /= sum;
        printf("Escalonado[%lf]: %lf\n", fitness[i], e );
    }

} 