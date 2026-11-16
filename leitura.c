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


int processar_comandos(Banco * banco, char * nome_arquivo, FILE * saida){

    FILE * arquivo_comandos = fopen(nome_arquivo, "r");

    if(arquivo_comandos == NULL){
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }

    int m = banco->num_recursos;

    // Criei um vetor para ser reaproveitado em todos os comandos RQ e RL
    int * vetor_comandos = (int *) malloc(m * sizeof(int));

    if(vetor_comandos == NULL){
        fprintf(stderr, "Erro: falha ao alocar memoria para os comandos\n");
        fclose(arquivo_comandos);
        return 0;
    }

    char linha[256];

    while(fgets(linha, sizeof(linha), arquivo_comandos) != NULL){

        linha[strcspn(linha, "\n")] = '\0';

        if(linha[0] == '\0'){
            continue;
        }

        char * comando = strtok(linha, " ");

        // Comando de imprimir o Estado
        if(strcmp(comando, "*") == 0){

            if(strtok(NULL, " ") != NULL){

                fprintf(stderr, "Erro: %s possui o comando * com valores a mais\n", nome_arquivo);
                free(vetor_comandos);
                fclose(arquivo_comandos);
                return 0;
            }

            imprimir_estado(banco, saida);
            continue;
        }

        if(strcmp(comando, "RQ") != 0 && strcmp(comando, "RL") != 0){

            fprintf(stderr, "Erro: %s possui comando desconhecido '%s'\n", nome_arquivo, comando);
            free(vetor_comandos);
            fclose(arquivo_comandos);
            return 0;
        }

        // numero do cliente
        char * texto = strtok(NULL, " ");
        if(!verificar_numero(texto)){

            fprintf(stderr, "Erro: %s possui comando %s sem um numero de cliente valido\n", nome_arquivo, comando);
            free(vetor_comandos);
            fclose(arquivo_comandos);
            return 0;
        }

        int cliente = atoi(texto);
        if(cliente >= banco->num_clientes){

            fprintf(stderr, "Erro: %s possui o cliente %d, mas so existem %d clientes\n", nome_arquivo, cliente, banco->num_clientes);
            free(vetor_comandos);
            fclose(arquivo_comandos);
            return 0;
        }

        // os m valores do pedido ou da devolucao
        for(int j = 0; j < m; j++){
            texto = strtok(NULL, " ");

            if(!verificar_numero(texto)){
                fprintf(stderr, "Erro: %s possui comando %s do cliente %d com valores invalidos ou faltando (esperado %d valores)\n", nome_arquivo, comando, cliente, m);
                free(vetor_comandos);
                fclose(arquivo_comandos);
                return 0;
            }

            vetor_comandos[j] = atoi(texto);
        }

        // nao pode sobrar valor
        if(strtok(NULL, " ") != NULL){
            fprintf(stderr, "Erro: %s possui comando %s do cliente %d com mais de %d valores\n", nome_arquivo, comando, cliente, m);
            free(vetor_comandos);
            fclose(arquivo_comandos);
            return 0;
        }

        if(strcmp(comando, "RQ") == 0){

            fazer_pedido(banco, cliente, vetor_comandos, saida);
        }

        else{

            fazer_devolucao(banco, cliente, vetor_comandos, saida);
        }
    }

    free(vetor_comandos);
    fclose(arquivo_comandos);
    return 1;
}