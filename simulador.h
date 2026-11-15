#ifndef SIMULADOR_H
#define SIMULADOR_H


#include <stdio.h>
#include "banker.h"


int cabe_ou_nao(int *pedido, int *limite, int tamanho);
int simulador_estado_seguro(Banco *banco);

#endif