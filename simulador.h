#ifndef SIMULADOR_H
#define SIMULADOR_H


#include <stdio.h>
#include "banker.h"


int cabe_ou_nao(int *pedido, int *limite, int tamanho);
int simulador_estado_seguro(Banco *banco);

void imprimir_vetor(FILE * saida, int * vetor, int tamanho);
void fazer_pedido(Banco * banco, int cliente, int * pedido, FILE * saida);
void fazer_devolucao(Banco * banco, int cliente, int * devolucao, FILE * saida);

#endif