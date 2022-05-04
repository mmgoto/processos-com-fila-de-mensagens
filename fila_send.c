/*Para executar o programa gcc -Wall fila_send.c -o send -lrt */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#define QUEUE "/test_queue"

// Variáveis globais.
double *results;
unsigned n_process;

// Criação da função PI.
float pi(int id) {
    long ID = (long)id;
	double sum = 0.0,x;
	unsigned long long int i;
    unsigned long long int passos = 1000000000;       // 1 Bilhão.
	double h=1.0/passos;
	for (i = ID + 1; i <= passos; i += n_process) {   
		x = h * ((double)i - 0.5); 
		sum += 4.0 / (1.0 + x*x); 
	}
	results[ID] = h*sum;
    return results[ID];
}

int main (int argc, char *argv[]) 
{ 
    mqd_t queue;                                      // Descritor da fila de mensagem.
    float msg;                                        // Mensagem que será enviada.
    int new_process;                                  

    if(argc==2)
    {
        n_process = atoi(argv[1]);
    }
    else
    {
        printf("Para executar o programa informe 1 parâmetro.\n"); 
        exit(1);
    }
    
    results = malloc(n_process*sizeof(double));

    // Abre a fila de mensagens se existir.
    if ((queue = mq_open(QUEUE, O_RDWR)) < 0)
    {
        perror("mq_open");
        exit(1);
    }

    printf ("Ola, sou o processo pai %5d\n", getpid());
    printf ("\n");

    for (int i=0;i<n_process;i++){
        
        msg = pi(i);                                  // Define o valor para a mensagem.
	    new_process = fork ();                        // Cria um novo processo.

        if (new_process == 0){ 
            printf ("Sou o %5d filho de %5d e estou\n", getpid(),getppid());
            break;
        }
    }

    sleep(1); 

    if ( new_process == 0 ) {                         // Processo filho.

        // Envia a mensagem.
        if(mq_send (queue, (void*) &msg, sizeof(msg), 0) <0) 
        {
            perror("mq_receive");
            exit(1);
        }
        
        printf("Mandando a mensagem com o valor: %f\n", msg);
        sleep(1);
        exit(1);

    } else {                                          // Processo pai.
        sleep(1);
        for (int i=0;i<n_process;i++)
		printf("O Processo filho %d terminou sua execução. \n ",wait(0));
    }
    
}
