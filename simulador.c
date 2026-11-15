#include "banker.h"
#include "simulador.h"

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
    return 0;
}