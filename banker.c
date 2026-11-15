#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leitura.h"
#include "banker.h"
#include "memoria.h"


void imprimir_estado(Banco * banco, FILE * saida){

    fprintf(saida, "MAXIMUM | ALLOCATION | NEED\n");

    for(int i = 0; i < banco->num_clientes; i++){
        for(int j = 0; j < banco->num_recursos; j++){
            fprintf(saida, "%d ",banco->max[i][j]);
        }
        fprintf(saida, "| ");

        for(int j = 0; j < banco->num_recursos; j++){
            fprintf(saida, "%d ",banco->allocation[i][j]);
        }
        fprintf(saida, "| ");

    for(int j = 0; j < banco->num_recursos; j++){
        if(j > 0){
            fprintf(saida, " ");
        }
        fprintf(saida, "%d", banco->need[i][j]);
    }
        fprintf(saida,"\n");
    }

    fprintf(saida,"AVAILABLE ");
    for(int j = 0; j < banco->num_recursos; j++){

        if(j > 0){
            fprintf(saida, " ");
        }
        fprintf(saida, "%d",banco->available[j]);
    }
    fprintf(saida,"\n");
}

int main(int argc, char *argv[]) {

    Banco banco = {0};

    // Usuario passou pelo menos um recurso
    if (argc < 2) {
        fprintf(stderr, "Erro: informe a quantidade de instancias de cada recurso. Exemplo: ./banker 10 5 7\n");
        return 1;
    }

    banco.num_recursos = argc - 1;

    banco.available = (int *) malloc(banco.num_recursos * sizeof(int));

    if (banco.available == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para available.\n");
        return 1;
    }

    // Validação de argumentos
    for (int i = 0; i < banco.num_recursos; i++) {

        if (!verificar_numero(argv[i + 1])) {

            fprintf(stderr, "Erro: argumento invalido '%s'. Use apenas inteiros nao negativos.\n", argv[i + 1]);            
            free(banco.available);
            liberar_banco(&banco);
            return 1;

        }

        banco.available[i] = atoi(argv[i + 1]);
    }

    if (!ler_clientes(&banco, "customer.txt")) {
        liberar_banco(&banco);
        return 1;
    }

    FILE * saida = fopen("result.txt","a");
    if(saida == NULL){
        fprintf(stderr,"Erro: nao foi possivel abrir o arquivo result.txt\n");
        return 0;
    }
    imprimir_estado(&banco,saida);

    liberar_banco(&banco);
    return 0;
}