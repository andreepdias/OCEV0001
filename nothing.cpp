
    void gerar_populacao_inicial(Operadores_Inteiros op){
        for(int i = 0; i < tamanho_pop; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                individuos[i][j] = op.locus_aleatorio();
            }
        }
    }
    void gerar_populacao_inicial(Operadores_Binarios op){
        for(int i = 0; i < tamanho_pop; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                individuos[i][j] = op.locus_aleatorio();
            }
        }
    }
    void gerar_populacao_inicial(Operadores_Reais op){
        for(int i = 0; i < tamanho_pop; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                individuos[i][j] = op.locus_aleatorio();
            }
        }
    }
    void print_individuos(){
        for(int i = 0; i < tamanho_pop; i++){
            for(int j = 0; j < tamanho_cromossomo; j++){
                cout << individuos[i][j] << "\t";
            }
            cout << endl;
        }
    }