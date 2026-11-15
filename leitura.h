#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memoria.h"
#include "banker.h"

int verificar_numero(char *str);
int ler_clientes(Banco * banco, char * nome_arquivo);


#endif