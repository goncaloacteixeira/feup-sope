# SOPE - Trabalho Prático nº 2 - Etapa 2 - T7G01

Trabalho realizado por:

- Gonçalo André Carneiro Teixeira (up201806562)
- João Carlos Machado Rocha Pires (up201806079)
- Maria Francisca Ferrão Castelo Branco de Faria e Almeida (up201806398)

De seguida, mencionam-se os pormenores de implementação que consideramos mais importantes.

## Modo de Aceitação de Pedidos

Os pedidos enviados por uma pthread \<n> para o servidor são aceites caso o tempo de utilização que o cliente define no request não ultrapassa o tempo restante de execução do servidor. Por outras palavras, todos os pedidos cuja duração de utilização seja superior ao tempo restante de execução do servidor serão rejeitados.

O que significa que todos os pedidos rejeitados serão marcados como "2LATE".

## Detalhes da Implementação

- Os pedidos de acesso são feitos pelo cliente no programa *multithread* com intervalos de `40 ms`, fazendo portanto 25 pedidos/segundo.
- As respostas do servidor são processadas imediatamente após determinar se o pedido é aceite ou rejeitado.

  - Quando o programa corre sem limitações, nomeadamente do nº máximo de threads por processo e do número máximo de lugares, o pedido do cliente terá uma resposta rapidamente; 
  - por outro lado, quando essas limitações são impostas o cliente irá demorar um pouco a obter a resposta. Uma vez que a duração máxima requisitada pelo cliente é de `100 ms` e o intervalo de pedidos é de `40 ms` o tempo que o cliente demora, no máximo, a receber uma resposta seria `60 ms`, correspondente ao tempo de utilização do pedido anterior retirando o tempo entre cada pedido. A nossa implementação contém um ciclo para tentar obter uma resposta do servidor, quando o número de tentativas máximo for ultrapassado assume-se que o servidor fechou e é emitido um "FAILD".
- Depois do servidor ter fechado, o programa do cliente continuará a ser executado, e todos os pedidos emitidos terão um resultado de "FAILD". Uma vez que o servidor executa o `unlink` do FIFO público, ao tentar abrir o FIFO no lado do cliente ocorrerá um erro.
- Lugares atribuídos:

  - caso o servidor não tenha limitações no número de lugares (i.e. lugares infinitos), todos os pedidos aceites terão o lugar atribuído igual a `1`, sabemos de início que existem sempre lugares, então ao colocar o lugar igual a `1` sabemos que o cliente foi aceite e que há pelo menos um lugar disponível, independente do número de pedidos a serem processados.
  - caso o servidor tenha limitações no número de lugares, o lugar X, caso esteja ocupado não poderá ser utilizado enquando não for libertado. Outra conclusão que podemos tirar é que no máximo estarão a ser utilizados N lugares (cenário realista). A forma como determinamos qual lugar utilizar será explicada na secção seguinte.
- Limitação do número de threads por processo

  - Utilizamos semáforos iniciados com o número máximo de threads por processo passado por argumento no programa do servidor. A sincronização é facilmente obtida com `wait` e `post` sempre que alguma thread é criada e quando acaba, respetivamente.
- Limitação do número de lugares máximo
  - Utilizamos mutexes e semáforos para limitar o número de lugares máximo no servidor, as mutexes para controlar o acesso a zona crítica (alterar a fila de lugares) e semáforos para controlar o número de lugares ocupados.

## Structs

### 1. Mensagens trocadas entre o Servidor e o Cliente

```c
typedef struct {
    /* ID do pedido -> começa em zero */
    int id;
    /* process ID no pedido atual */
    pid_t pid;
    /* thread ID no pedido atual */
    pthread_t tid;
    /* duração do acesso ao servidor (pedido pelo cliente) em milliseconds */
    int dur;
    /* posição atribuida pelo servidor ao cliente (-1 no pedido) */
    int pl;
} message_t;
```

### 2. Argumentos do Cliente

```c
typedef struct {
    int seconds;
    char* server_fifo;
} client_args_t;
```

### 3. Argumentos do Servidor

```c
typedef struct {
    int seconds;
    int nplaces;
    int nthreads;
    char* fifoname;
} server_args_t;
```

### 4. Fila de Lugares

A Fila de Lugares foi implementada com base numa `queue` normal, à cabeça tem o próximo lugar disponível. A implementação é bastante simples.

[Ficheiro places.h](./places.h)

```c
typedef struct {
    int front; 		/* index da queue correspondente à cabeça da fila */
    int back;  		/* index da queue correspondente à cauda da fila */
    int size;		/* tamanho atual da fila */
    int max_size;	/* tamanho máximo da fila (corresponde ao nº max. de lugares) */
    int* queue;		/* array de lugares */
} places_t;
```

Com esta estrutura conseguimos manter o registo de lugares utilizados e prontos a ser utilizados, basta preencher a fila com o tamanho máximo igual ao número de lugares máximo imposto pelo servidor e ir retirando ou adicionado lugares à medida que um pedido é acabado de processar ou iniciado, respetivamente.

## Funções

### 1. log_message()

Função para emitir uma mensagem no `stdout`, tendo em conta os campos da mensagem. 

```c
void log_message(int i, pid_t pid, pthread_t tid, int dur, int pl, char *oper);
```

Os valores para `oper` são os seguintes:

- Client-side: `"IAMIN"`, `"CLOSD"`, `"FAILD"`

- Server-side: `"ENTER"`, `"TIMUP"`, `"2LATE"`, `"GAVUP"`

### 2. delta()

Função responsável por determinar há quanto tempo o programa está a ser executado, em milissegundos (útil para verificar se o tempo de execução chegou ao fim)

```c
double delta();
```



Estas funções estão declaradas em [utils.h](./utils.h) e implementadas em [utils.c](./utils.c)

## Linhas de código relevantes

- Geração de durações (pseudo)aleatórias para o acesso do cliente ao quarto de banho, em milisegundos.

```c
request.dur = (rand() % (100 - 20 + 1)) + 20;
```

- Detach da pthread dentro da função de thread:

```c
pthread_detach(tid = pthread_self());
```



# Notas Finais

O programa é compilado com auxílio de um [Makefile](./Makefile).

Tal como é formulado no enunciado:

- build

```bash
make
```

- Run Client Side:

```bash
./U2 -t <exec time> <fifoname>
```

- Run Server Side:

```bash
./Q2 -t <exec tim> [-n <nthreads>] [-l <nplaces>] <fifoname>
```



### Adicional

#### Extra 1

Criamos um script de testes, que talvez sirva para para o utilizador entender o nosso ponto de vista em relação à conceção dos programas. [test](./test)

Pode ser necessário atribuir permissões ao script:

```bash
chmod +x test
```

Para correr o teste:

```bash
./test [-q <server time>] [-u <client time>] [-l <nplaces>] [-n <nthreads>] [-f <fifo>]
```

Por defeito:

- Server time: 10s
- Client time: 15s
- nplaces: 0 (unlimited)
- nthreads: 0 (unlimited)
- fifo: fifo.server

Exemplo de output:

```bash
Setting server timeout to 10sec
Setting client timeout to 15sec
Setting max threads to 0
Setting max places to 0
SERVER/CLIENT RUNNING ...
END OF SERVER/CLIENT
Requests sent: 249
Failed requests: 0
Gave up requests: 0
[PASSED] 2LATE - too late requests: 1
[PASSED] ENTER - accepted requests: 248
[PASSED] TIMEUP - fulfilled requests: 248
```



#### Extra 2

O `Makefile` tem um job `test` que compila o projeto; faz várias chamadas diferentes ao script de testes; aponta o número de testes bem sucedidos e falhados; limpa o diretório. Estes testes demoram cerca de 6 minutos a concluir.
