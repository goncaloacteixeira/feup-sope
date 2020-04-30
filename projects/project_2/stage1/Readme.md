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

- Criaçaõ de uma struct para armazenar as informações de um pedido ou de uma resposta

```C
typedef struct {
    /* ID do pedido -> começa em zero */
    int id;
    /* process ID no pedido atual */
    pid_t pid;
    /* thread ID no pedido atual */
    int tid;
    /* duração do acesso ao servidor (pedido pelo cliente) em microseconds (para facilitar o uso de usleep) */
    int dur;
    /* posição atribuida pelo servidor ao cliente (-1 no pedido) */
    int pl;
} message_t ;
```

## Funções

- Criação de uma função responsável por registar as operações na stdout

```C
void log_message(int i, pid_t pid, pid_t tid, int dur, int pl, char *oper);
```

- Criação de um função responsável por devolver há quanto tempo o programa corre e assim verificar se já atingiu o final da execução

```C
double delta();
```

## Linhas de código relevantes

- Geração de durações (pseudo)aleatórias para o acesso do cliente ao quarto de banho, em milisegundos

```C
request.dur = (rand() % (100 - 20 + 1)) + 20;
``` 
