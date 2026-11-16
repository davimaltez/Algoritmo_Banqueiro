#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memoria.h"
#include "banker.h"
#include "simulador.h"

int verificar_numero(char *str);
int ler_clientes(Banco * banco, char * nome_arquivo);
int processar_comandos(Banco * banco, char * nome_arquivo, FILE * saida);
void imprimir_estado(Banco * banco, FILE * saida);

#endif