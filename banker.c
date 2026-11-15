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


int ler_clientes(Banco * banco, char * nome_arquivo){
   
    FILE * arquivo_clientes = fopen(nome_arquivo,"r");
    if(arquivo_clientes == NULL){
        fprintf(stderr,"Erro: nao foi possivel abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }

    //contagem de clientes
    char linha[256];
    while(fgets(linha,sizeof(linha),arquivo_clientes) != NULL){

        linha[strcspn(linha, "\n")] = '\0';

        if(linha[0] != '\0'){

            banco->num_clientes++;
        }
    }

    if(banco->num_clientes == 0){
        fprintf(stderr,"Erro: arquivo %s esta vazio\n", nome_arquivo);
        fclose(arquivo_clientes);
        return 0;
    }

    rewind(arquivo_clientes);

    banco->max = alocar_matriz(banco->num_clientes, banco->num_recursos);
    banco->allocation = alocar_matriz(banco->num_clientes, banco->num_recursos);
    banco->need = alocar_matriz(banco->num_clientes, banco->num_recursos);

    if (banco->max == NULL || banco->allocation == NULL || banco->need == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para as matrizes\n");
        fclose(arquivo_clientes);
        return 0;
    }

    //Leitura dos números
    int i = 0;      

    while (fgets(linha, sizeof(linha), arquivo_clientes) != NULL) {

        linha[strcspn(linha, "\n")] = '\0';

        if (linha[0] == '\0') {
            continue; 
        }

        int j = 0; // indice do recurso (coluna)

        char *numero = strtok(linha, ",");

        while (numero != NULL) {

            if (!verificar_numero(numero)) {
                fprintf(stderr, "Erro: %s possui valor invalido '%s'\n", nome_arquivo, numero);
                fclose(arquivo_clientes);
                return 0;
            }

            if (j >= banco->num_recursos) {
                fprintf(stderr, "Erro: %s possui cliente com mais de %d valores (quantidade de recursos passada na execucao)\n", nome_arquivo, banco->num_recursos);
                fclose(arquivo_clientes);
                return 0;
            }

            banco->max[i][j] = atoi(numero);
            banco->need[i][j] = banco->max[i][j]; // no inicio ninguem tem nada

            j++;
            numero = strtok(NULL, ",");
        }

        if (j != banco->num_recursos) {
            fprintf(stderr, "Erro: %s possui cliente com %d valores, mas foram passados %d recursos na execucao\n", nome_arquivo, j, banco->num_recursos);
            fclose(arquivo_clientes);
            return 0;
        }

        i++;
    }

    fclose(arquivo_clientes);
    return 1;
}


void imprimir_estado(Banco * banco, FILE * saida){

    fprintf(saida, "MAXIMUM | ALLOCATION | NEED\n");

    for(int i = 0; i < banco->num_clientes; i++){
        for(int j = 0; j < banco->num_recursos; j++){
            fprintf(saida, "%d ",banco->max[i][j]);
        }
        fprintf(saida, "| ");

        for(int j = 0; j < banco->num_recursos; j++){
            fprintf(saida, "%d ",banco->allocation[i][j]);
        }
        fprintf(saida, "| ");

    for(int j = 0; j < banco->num_recursos; j++){
        if(j > 0){
            fprintf(saida, " ");
        }
        fprintf(saida, "%d", banco->need[i][j]);
    }
        fprintf(saida,"\n");
    }

    fprintf(saida,"AVAILABLE ");
    for(int j = 0; j < banco->num_recursos; j++){

        if(j > 0){
            fprintf(saida, " ");
        }
        fprintf(saida, "%d",banco->available[j]);
    }
    fprintf(saida,"\n");
}

int main(int argc, char *argv[]) {

    Banco banco = {0};

    // Usuario passou pelo menos um recurso
    if (argc < 2) {
        fprintf(stderr, "Erro: informe a quantidade de instancias de cada recurso. Exemplo: ./banker 10 5 7\n");
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

            fprintf(stderr, "Erro: argumento invalido '%s'. Use apenas inteiros nao negativos.\n", argv[i + 1]);            
            free(banco.available);
            liberar_banco(&banco);
            return 1;

        }

        banco.available[i] = atoi(argv[i + 1]);
    }

    if (!ler_clientes(&banco, "customer.txt")) {
        liberar_banco(&banco);
        return 1;
    }

    FILE * saida = fopen("result.txt","a");
    if(saida == NULL){
        fprintf(stderr,"Erro: nao foi possivel abrir o arquivo result.txt\n");
        return 0;
    }
    imprimir_estado(&banco,saida);

    liberar_banco(&banco);
    return 0;
}