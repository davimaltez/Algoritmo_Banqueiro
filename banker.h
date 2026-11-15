#ifndef BANKER_H
#define BANKER_H

#include <stdio.h>

typedef struct {
    int num_clientes;
    int num_recursos;
    int *available;
    int **max;
    int **allocation;
    int **need;
} Banco;

#endif