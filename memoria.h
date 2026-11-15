#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include "banker.h"


int **alocar_matriz(int linhas, int colunas);
void liberar_matriz(int **mat, int linhas);
void liberar_banco(Banco *banco);

#endif
