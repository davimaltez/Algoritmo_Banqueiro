#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int num_clientes;
    int num_recursos;
    int *available;
    int **max;
    int **allocation;
    int **need;
} Banco;


int verificar_numero(char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    int tamanho = 0;

    for (int i = 0; str[i] != '\0'; i++) {

        if (str[i] < '0' || str[i] > '9') {
            return 0; // caso ache alguma letra, sinal negativo ou simbolo
        }
        tamanho++;
    }

    //verificação para número grande demais
    if(tamanho > 9){
        return 0; 
    }

    return 1;
}

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

                free(mat[j]);
            }
            free(mat);
            return NULL;
        }
    }

    return mat;
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

int main(int argc, char *argv[]) {

    Banco banco = {0};

    // Usuario passou pelo menos um recurso
    if (argc < 2) {
        fprintf(stderr, "Erro: informe a quantidade de instancias de cada recurso. Exemplo: ./banker 10 5 7\n");
        return 1;
    }

    int num_recursos = argc - 1;

    // Aloca o vetor available dinamicamente de acordo com quantos argumentos vieram
    int *available = (int *) malloc(num_recursos * sizeof(int));

    if (available == NULL) {
        printf("Erro ao alocar memoria para available.\n");
        return 1;
    }

    banco.num_recursos = argc - 1;

    banco.available = (int *) malloc(banco.num_recursos * sizeof(int));

    if (banco.available == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para available.\n");
        return 1;
    }

    // Validação de argumentos
    for (int i = 0; i < banco.num_recursos; i++) {

        if (!verificar_numero(argv[i + 1])) {

            fprintf(stderr, "Erro: argumento invalido '%s'. Use apenas inteiros nao negativos.\n", argv[i + 1]);            free(available);
            return 1;

        }

        available[i] = atoi(argv[i + 1]);
    }


    liberar_banco(&banco);
    return 0;
}