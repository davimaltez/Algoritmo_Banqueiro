#include <stdio.h>
#include "leitura.h"


int verificar_numero(char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    int tamanho = 0;

    for (int i = 0; str[i] != '\0'; i++) {

        if (str[i] < '0' || str[i] > '9') {
            return 0; // caso ache alguma letra, sinal negativo ou simbolo
        }
        tamanho++;
    }

    //verificação para número grande demais
    if(tamanho > 9){
        return 0; 
    }

    return 1;
}

int ler_clientes(Banco * banco, char * nome_arquivo){
   
    FILE * arquivo_clientes = fopen(nome_arquivo,"r");
    if(arquivo_clientes == NULL){
        fprintf(stderr,"Erro: nao foi possivel abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }

    //contagem de clientes
    char linha[256];
    while(fgets(linha,sizeof(linha),arquivo_clientes) != NULL){

        linha[strcspn(linha, "\n")] = '\0';

        if(linha[0] != '\0'){

            banco->num_clientes++;
        }
    }

    if(banco->num_clientes == 0){
        fprintf(stderr,"Erro: arquivo %s esta vazio\n", nome_arquivo);
        fclose(arquivo_clientes);
        return 0;
    }

    rewind(arquivo_clientes);

    banco->max = alocar_matriz(banco->num_clientes, banco->num_recursos);
    banco->allocation = alocar_matriz(banco->num_clientes, banco->num_recursos);
    banco->need = alocar_matriz(banco->num_clientes, banco->num_recursos);

    if (banco->max == NULL || banco->allocation == NULL || banco->need == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para as matrizes\n");
        fclose(arquivo_clientes);
        return 0;
    }

    //Leitura dos números
    int i = 0;      

    while (fgets(linha, sizeof(linha), arquivo_clientes) != NULL) {

        linha[strcspn(linha, "\n")] = '\0';

        if (linha[0] == '\0') {
            continue; 
        }

        int j = 0; // indice do recurso (coluna)

        char *numero = strtok(linha, ",");

        while (numero != NULL) {

            if (!verificar_numero(numero)) {
                fprintf(stderr, "Erro: %s possui valor invalido '%s'\n", nome_arquivo, numero);
                fclose(arquivo_clientes);
                return 0;
            }

            if (j >= banco->num_recursos) {
                fprintf(stderr, "Erro: %s possui cliente com mais de %d valores (quantidade de recursos passada na execucao)\n", nome_arquivo, banco->num_recursos);
                fclose(arquivo_clientes);
                return 0;
            }

            banco->max[i][j] = atoi(numero);
            banco->need[i][j] = banco->max[i][j]; // no inicio ninguem tem nada

            j++;
            numero = strtok(NULL, ",");
        }

        if (j != banco->num_recursos) {
            fprintf(stderr, "Erro: %s possui cliente com %d valores, mas foram passados %d recursos na execucao\n", nome_arquivo, j, banco->num_recursos);
            fclose(arquivo_clientes);
            return 0;
        }

        i++;
    }

    fclose(arquivo_clientes);
    return 1;
}