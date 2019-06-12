build:
	g++ source/util.cpp source/parametros.cpp source/individuo.cpp source/populacao.cpp  source/main.cpp -o s -fopenmp -O3

debug:
	g++ source/util.cpp source/parametros.cpp source/individuo.cpp source/populacao.cpp  source/main.cpp -fopenmp -O3 -g