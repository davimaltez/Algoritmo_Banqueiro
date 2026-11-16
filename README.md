# Algoritmo_Banqueiro

Implementação 5 de Infraestrutura de Software, CESAR School.
Autor: Davi Maltez Costa Vasconcelos

## O que o programa faz

Implementa em C o algoritmo do banqueiro (Silberschatz, *Operating System Concepts*, 10ª ed., seção 8.6.3) para evitar deadlock. O programa lê a quantidade de instâncias de cada recurso pela linha de comando, o máximo que cada cliente pode pedir em `customer.txt`, e uma lista de comandos em `commands.txt`. Para cada pedido (`RQ`), ele só concede os recursos se o sistema continuar em **estado seguro**. O resultado de cada comando é escrito em `result.txt`.

O número de clientes (linhas do `customer.txt`) e o número de tipos de recurso (argumentos da linha de comando) não são fixos.

## Como compilar e executar

Requisitos: `gcc` e `make`, em Linux, Unix ou macOS.

```
make                  # compila e gera o executável banker
./banker 10 5 7       # executa com 3 tipos de recurso: 10, 5 e 7 instâncias
make clean            # remove o executável
```

O `customer.txt` e o `commands.txt` precisam estar na **mesma pasta** de onde o programa é executado. O resultado é gravado em `result.txt`, na mesma pasta. O arquivo é recriado a cada execução.

## Formato dos arquivos

**Argumentos:** cada argumento é a quantidade de instâncias de um tipo de recurso, e a quantidade de argumentos define o número de tipos de recurso. Exemplo: `./banker 10 5 7`.

**`customer.txt`:** uma linha por cliente, com o máximo de cada recurso separado por vírgula e sem espaços. O número de valores em cada linha precisa ser igual ao número de argumentos.

```
7,5,3
3,2,2
9,0,2
2,2,2
4,3,3
```

**`commands.txt`:** um comando por linha, com os campos separados por espaço.

| Comando | Significado |
|---|---|
| `RQ i a b c` | o cliente `i` solicita `a b c` instâncias de cada recurso |
| `RL i a b c` | o cliente `i` devolve `a b c` instâncias de cada recurso |
| `*` | imprime o estado atual (máximo, alocação, necessidade e disponível) |

Os clientes são numerados a partir de 0, e cada comando precisa ter exatamente um valor por tipo de recurso.

## Saída (`result.txt`)

Uma mensagem por comando:

```
Allocate to customer 0 the resources 0 2 1
Release from customer 0 the resources 0 1 1
The resources 3 3 2 are not enough to customer 4 request 4 0 0
The customer 0 request 0 2 0 was denied because result in an unsafe state
The customer 2 request 0 1 0 was denied because exceed its maximum allocation
```

O comando `*` imprime a tabela:

```
MAXIMUM | ALLOCATION | NEED
7 5 3 | 0 1 0 | 7 4 3
...
AVAILABLE 3 3 2
```

## Como o pedido é tratado

Para cada `RQ`, na ordem do livro:

1. Se o pedido é maior que a necessidade (`need`) do cliente, é negado (`exceed its maximum allocation`).
2. Se o pedido é maior que o disponível (`available`), é negado (`are not enough`).
3. O pedido é aplicado ao estado e o algoritmo de segurança é executado. Se o estado for seguro, o pedido é concedido. Se for inseguro, o estado anterior é restaurado e o pedido é negado (`result in an unsafe state`).

O algoritmo de segurança trabalha em uma **cópia** do vetor `available` e não altera o estado real.

## Tratamento de erros

Em qualquer erro de entrada, o programa escreve uma mensagem em `stderr`, libera a memória e termina com código de saída `1`. Em execução normal, o código é `0`. Casos tratados:

- número de argumentos igual a zero, ou diferente do número de colunas do `customer.txt`;
- argumento que não seja inteiro não negativo (letra, sinal, vazio ou com mais de 9 dígitos);
- `customer.txt` ou `commands.txt` inexistente, que não possa ser aberto, ou `customer.txt` vazio;
- linha do `customer.txt` com valor inválido, ou com mais ou menos valores que o número de recursos;
- comando desconhecido, cliente inexistente, valores inválidos, faltando ou sobrando no `commands.txt`.

## Decisões sobre casos que o enunciado não define

- **Devolução maior que a alocação do cliente:** é recusada, sem encerrar o programa, com a mensagem `The customer i release ... was denied because exceed its allocation`. A alternativa deixaria a alocação negativa.
- **Comando inválido no `commands.txt`:** o programa encerra com erro nessa linha, como pede o enunciado. As linhas anteriores permanecem no `result.txt`.
- **Linhas em branco:** são ignoradas nos dois arquivos.
- **`commands.txt` vazio:** não é erro. O `result.txt` fica vazio e o código de saída é `0`.
- **Espaços no `customer.txt`:** não são aceitos, porque o formato do enunciado não tem espaços (`7, 5, 3` é inválido). No `commands.txt`, espaços repetidos entre os campos são aceitos.
- **Final de linha:** os arquivos devem usar quebra de linha no padrão Linux (`\n`). Arquivos com `\r\n` (Windows) não são tratados.
- **Máximo maior que o total do sistema:** o livro diz que isso não é permitido, mas o enunciado não pede essa validação, então ela não é feita.
- **Limites:** linhas de até 255 caracteres, e valores de até 9 dígitos.

## Diferença em relação ao exemplo do enunciado

No exemplo do enunciado, a mensagem do pedido `RQ 4 4 0 0` mostra `The resources 3 2 2 are not enough ...`. Porém, o `*` impresso logo antes mostra `AVAILABLE 3 3 2`, e a soma das alocações confirma que o disponível naquele momento é `3 3 2`. O programa imprime o valor calculado do estado real, `3 3 2`. As demais linhas do exemplo são idênticas.

## Organização do código

| Arquivo | Conteúdo |
|---|---|
| `banker.h` | definição da struct `Banco` (`available`, `max`, `allocation`, `need`) |
| `banker.c` | função `main` |
| `leitura.c` / `leitura.h` | validação de números, leitura do `customer.txt` e execução do `commands.txt` |
| `memoria.c` / `memoria.h` | alocação e liberação das matrizes e do `Banco` |
| `simulador.c` / `simulador.h` | algoritmo de segurança, tratamento de pedido (`RQ`) e devolução (`RL`), impressão do estado |
| `Makefile` | compilação e limpeza |
| `testes.sh` | bateria de testes, que gera o `evidencias.log` |
| `evidencias.log` | saída dos testes |

