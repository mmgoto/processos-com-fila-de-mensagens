#### CEFET-MG - Engenharia de Computação

#### Sistemas Operacionais

---

# Processos com Fila de Mensagens

O objetivo deste trabalho é entendermos melhor o funcionamento dos sistemas operacionais e como funciona a criação de processos que se comunicam através de uma fila de mensagens

## Metodologia

Para o desenvolvimento desta atividade foi necessário criar 2 programas: um receptor e um emissor.

##### Memória Compartilhada

1.  **Criando a fila de mensagens no receptor**

Para que os programas possam se comunicar, criamos uma fila de mensagens que pode ser acessada pelo receptor. Caso haja algum erro durante a criação, ele imprimirá o erro na tela..
O receptor recebe as mensagens de acordo com suas prioridades(inexistentes nessa atividade).

```C
   if ((queue = mq_open(QUEUE, O_RDWR|O_CREAT, 0666, &attr)) < 0)
    {
        perror("mq_open");
        exit(1);
    }
```

2. **Recebendo mensagens**

Foi criado um loop infinito que recebe os dados da fila assim que ela é atualizada.
Em seguida, os valores recebidos são somados e exibidos no terminal.

```C
 if((mq_receive (queue, (void*) &msg, 1024, 0)) <0)
        {
            perror("mq_receive");
            exit(1);
        }
        printf("\n");
        printf("Recebendo a mensagem com o valor: %f\n", msg);
        printf("Somatório parcial de PI é: %f\n", sum += msg);
    }
```

3. **Acessando a fila e enviando informações por ela**

Para armazenar os valores parciais de _pi_ foi criada uma variável global, que é alocada dinamicamente.
Como no programa receptor a fila é criada e fica aberta, no emissor ela é acessada e utilizada para enviar os valores parciais de pi.

```C
    results = malloc(n_process*sizeof(double));

    if ((queue = mq_open(QUEUE, O_RDWR)) < 0)
    {
        perror("mq_open");
        exit(1);
    }
```

Para realizar os cálculos foi criado um loop que cria processos filhos que enviam os valores para o receptor.

```C
    for (int i=0;i<n_process;i++){

        msg = pi(i);
	    new_process = fork ();

        if (new_process == 0){
            printf ("Sou o %5d filho de %5d e estou\n", getpid(),getppid());
            break;
        }
    }
```

Para isso ela utiliza um comando que tem como parâmetro a prioridade da mensagem que foi definida como 0, para evitar diferenças de prioridade.

```C
        if(mq_send (queue, (void*) &msg, sizeof(msg), 0) <0)
        {
            perror("mq_receive");
            exit(1);
        }
```

# Resultado

1. **Receptor**

Para compilação do receptor devemos utilizar:

> gcc -Wall fila_recv.c -o recv -lrt

Para iniciarmos devemos utilizar:

> ./recv

2. **Emissor**

Para compilação do emissor devemos utilizar:

> gcc -Wall fila_send.c -o send -lrt

Para iniciarmos devemos utilizar:

> ./send **x**

em que **x** deve ser um valor inteiro, representando a quantidade de processos desejados.
