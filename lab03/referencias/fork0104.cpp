#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


#define BUFFER 255

int Strlen(char string[]);
int main(void)
{
    int fd[2]; /* File descriptors pro Pipe*/
    pid_t pid; /* Vari�vel para armazenar o pid*/

    /* Criando nosso Pipe */
    if(pipe(fd)<0) {
        perror("pipe") ;
        return -1 ;
    }

    /* Criando o processo filho*/
    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
 /* Processo Pai*/
    if (pid > 0)
    {
        /*No pai, vamos ESCREVER, ent�o vamos fechar a LEITURA do Pipe neste lado*/
        close(fd[0]);
        int len;
        char str1[BUFFER] =  "Aprendi a usar Pipes em C!";

        len =  Strlen(str1);

        printf("\nString enviada pelo pai no Pipe: '%s'\n\n", str1);


        /* Escrevendo a string no pipe */
        write(fd[1], str1, len + 1);
        sleep(1);
        exit(0);
    }
    /* Processo Filho*/
    else
    {
        char str_recebida[BUFFER];

        /* No filho, vamos ler. Ent�o vamos fechar a entrada de ESCRITA do pipe */
        close(fd[1]);

        /* Lendo o que foi escrito no pipe, e armazenando isso em 'str_recebida' */
        read(fd[0], str_recebida, sizeof(str_recebida));

        printf("\nString lida pelo filho no Pipe : '%s'\n\n", str_recebida);
        exit(0);
    }
    return(0);
}

int Strlen(char string[])       /*Definição da função*/
 {
   int i;
   for (i=0; string[i] != '\0'; i++)
      ;
   return (i);
}
