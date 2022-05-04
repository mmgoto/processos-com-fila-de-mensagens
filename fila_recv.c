/*Para executar o programa gcc -Wall fila_recv.c -o recv -lrt */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>

#define QUEUE "/test_queue"

int main (int argc, char *argv[]) 
{ 
    mqd_t queue;                                // Descritor da fila de mensagens.  
    struct mq_attr attr;                        // Atributos da fila de mensagens.
    float msg;                                  // Mensagens contendo o seu tipo.
    float sum = 0;

    attr.mq_maxmsg = 100;                       // Capacidade para 100 mensagens. 
    attr.mq_flags = 0;                          // Flags para informar se a fila é bloqueante.

    umask(0);                                   // Máscara de permissões.
    
    // Abre ou cria a fila com a permissão 0666(read and write for all).
    if ((queue = mq_open(QUEUE, O_RDWR|O_CREAT, 0666, &attr)) < 0)
    {
        perror("mq_open");
        exit(1);
    }
    
    while(1) 
    {

        // Recebe cada mensagem e imprime seu conteúdo.
        if((mq_receive (queue, (void*) &msg, 1024, 0)) <0) 
        {
            perror("mq_receive");
            exit(1);
        }
        printf("\n");
        printf("Recebendo a mensagem com o valor: %f\n", msg);
        printf("Somatório parcial de PI é: %f\n", sum += msg);
    }

}
