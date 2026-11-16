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

            //se o cliente i ainda não terminou e o que ele ainda precisa cabe no cofre de rascunho
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

void imprimir_vetor(FILE * saida, int * vetor, int tamanho){

    for(int j = 0; j < tamanho; j++){
        if(j > 0){
            fprintf(saida, " ");
        }
        fprintf(saida, "%d", vetor[j]);
    }
}

void fazer_pedido(Banco * banco, int cliente, int * pedido, FILE * saida){

    int m = banco->num_recursos;


    // O pedido passa do que o cliente ainda pode pedir?
    if(!cabe_ou_nao(pedido, banco->need[cliente], m)){

        fprintf(saida, "The customer %d request ", cliente);
        imprimir_vetor(saida, pedido, m);
        fprintf(saida, " was denied because exceed its maximum allocation\n");
        return;
    }

    // Verifica se tem isso tudo no cofre agora
    if(!cabe_ou_nao(pedido, banco->available, m)){
        
        fprintf(saida, "The resources ");
        imprimir_vetor(saida, banco->available, m);
        fprintf(saida, " are not enough to customer %d request ", cliente);
        imprimir_vetor(saida, pedido, m);
        fprintf(saida, "\n");
        return;
    }

    // Aplica o pedido
    for(int j = 0; j < m; j++){
        banco->available[j] -= pedido[j];
        banco->allocation[cliente][j] += pedido[j];
        banco->need[cliente][j] -= pedido[j];
    }

    // O estado com o pedido aplicado e seguro?
    if(simular_estado_seguro(banco)){
        fprintf(saida, "Allocate to customer %d the resources ", cliente);
        imprimir_vetor(saida, pedido, m);
        fprintf(saida, "\n");
    }
    else{
        //Inseguro: desfaz tudo, com os sinais trocados
        for(int j = 0; j < m; j++){

            banco->available[j] += pedido[j];
            banco->allocation[cliente][j] -= pedido[j];
            banco->need[cliente][j] += pedido[j];
        }

        fprintf(saida, "The customer %d request ", cliente);
        imprimir_vetor(saida, pedido, m);
        fprintf(saida, " was denied because result in an unsafe state\n");
    }
}