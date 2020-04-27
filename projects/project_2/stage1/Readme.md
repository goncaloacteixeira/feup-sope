# SOPE - Trabalho Prático nº 2 - Etapa 1 - T7G01

Trabalho realizado por:

- Gonçalo André Carneiro Teixeira (up201806562)
- João Carlos Machado Rocha Pires (up201806079)
- Maria Francisca Ferrão Castelo Branco de Faria e Almeida (up201806398)

De seguida, mencionam-se os pormenores de implementação que consideramos mais importantes.

## Structs

- Criação de uma struct para armazenar os argumentos passados pelo programa U1

```C
typedef struct {
    int seconds;
    char* server_fifo;
} client_args_t;
```
- Criação de uma struct para armazenar os argumentos passados pelo programa Q2

```C
typedef struct {
    int seconds;
    char* server_fifo;
} client_args_t;
```

## Funções

- Criação de uma função responsável por registar as operações na stdout

```C
void log_message(int i, pid_t pid, pid_t tid, int dur, int pl, char *oper);
```

## Linhas de código relevantes

- Geração de durações (pseudo)aleatórias para o acesso do cliente ao quarto de banho

```C
int microseconds = 1000;
request.dur = (rand() % ((5 * microseconds) - (1 * microseconds) + 1)) + (1 * microseconds);
``` 

Nota: A duração está a ser gerada em microsegundos uma vez que estamos a usar a função usleep.