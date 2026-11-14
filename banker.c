#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int verificar_numero(char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    for (int i = 0; str[i] != '\0'; i++) {

        if (str[i] < '0' || str[i] > '9') {
            return 0; // caso ache alguma letra, sinal negativo ou simbolo
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
    // Usuario passou pelo menos um recurso
    if (argc < 2) {
        printf("Erro: informe a quantidade de instancias dos recursos na execucao.\n");
        return 1;
    }

    int num_recursos = argc - 1;

    // Aloca o vetor available dinamicamente de acordo com quantos argumentos vieram
    int *available = (int *) malloc(num_recursos * sizeof(int));

    if (available == NULL) {
        printf("Erro ao alocar memoria para available.\n");
        return 1;
    }

    // Valida cada argumento e guarda no vetor available
    for (int i = 0; i < num_recursos; i++) {

        if (!verificar_numero(argv[i + 1])) {

            printf("Erro: argumento invalido '%s'. Use apenas inteiros nao negativos.\n", argv[i + 1]);
            free(available);
            return 1;
        }

        available[i] = atoi(argv[i + 1]);
    }

    printf("Recursos carregados (%d tipos): ", num_recursos);
    for (int i = 0; i < num_recursos; i++) {
        printf("%d ", available[i]);
    }
    printf("\n");

    int num_clientes = 0;
    int **max = NULL;
    int **allocation = NULL;
    int **need = NULL;

    // libera memoria antes de encerrar
    free(available);

    return 0;
}