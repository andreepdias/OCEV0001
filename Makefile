all:
	g++ parametros.cpp relatorio.cpp populacao.cpp dominios.cpp selecoes.cpp crossover.cpp mutacao.cpp problemas.cpp  main.cpp -o s -fopenmp -lsfml-graphics -lsfml-window -lsfml-system