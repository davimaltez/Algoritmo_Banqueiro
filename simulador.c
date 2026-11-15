#include "banker.h"
#include "simulador.h"
#include <stdlib.h>

int cabe_ou_nao(int *pedido, int *limite, int tamanho){

    //Se coube, enão retorna 1 
    //Se não coube, então retorna 0
    int coube = 1;
    for(int i = 0; i < tamanho; i++){
        if(pedido[i] > limite[i]){
            coube = 0;
            return coube;
        }
    }

    return coube;
}

int simular_estado_seguro(Banco * banco){

    // copia de verdade do available: a simulacao mexe so nela
    int * copia_available = (int *)malloc(banco->num_recursos * sizeof(int));

    // um "ja terminou?" por cliente, comecando todo com 0
    int * finish = (int *)calloc(banco->num_clientes, sizeof(int));

    if(copia_available == NULL || finish == NULL){
        fprintf(stderr, "Erro: falha ao alocar memoria na verificacao de seguranca\n");
        free(copia_available);
        free(finish);
        return 0;
    }

    for(int j = 0; j < banco->num_recursos; j++){
        copia_available[j] = banco->available[j];
    }

    int achou;

    do{
        achou = 0;

        for(int i = 0; i < banco->num_clientes; i++){

            if(finish[i] == 0 && cabe_ou_nao(banco->need[i], copia_available, banco->num_recursos)){

                // o cliente termina e devolve tudo o que tinha
                for(int j = 0; j < banco->num_recursos; j++){
                    copia_available[j] += banco->allocation[i][j];
                }

                finish[i] = 1;
                achou = 1;
            }
        }
    } while(achou);

    // seguro so se todos conseguiram terminar
    int seguro = 1;
    for(int i = 0; i < banco->num_clientes; i++){
        if(finish[i] == 0){
            seguro = 0;
        }
    }

    free(copia_available);
    free(finish);

    return seguro;
}