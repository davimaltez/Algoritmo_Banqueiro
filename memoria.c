#include "banker.h"
#include "memoria.h"


int **alocar_matriz(int linhas, int colunas) {
    int **matriz = (int **) malloc(linhas * sizeof(int *));
    if (matriz == NULL) {
        return NULL;
    }

    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *) calloc(colunas, sizeof(int));

        //deu ruim
        if (matriz[i] == NULL) {
            
            for (int j = 0; j < i; j++) {

                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }

    return matriz;
}

void liberar_matriz(int **mat, int linhas) {
    if (mat == NULL) {
        return;
    }
    for (int i = 0; i < linhas; i++) {
        free(mat[i]);
    }
    free(mat);
}

void liberar_banco(Banco *banco) {
    free(banco->available);
    liberar_matriz(banco->max, banco->num_clientes);
    liberar_matriz(banco->allocation, banco->num_clientes);
    liberar_matriz(banco->need, banco->num_clientes);
}