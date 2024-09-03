/*
EXERCÍCIO 1. Escreva um programa em C que crie um processo filho utilizando fork().
No processo pai, imprima o PID do processo pai e do filho.
No processo filho, imprima uma mensagem informando que ele é o filho e o PID dele.
Adicione um código no processo filho para fazer com que ele execute um loop que imprime uma mensagem 5 vezes, com um intervalo de 1 segundo entre as mensagens.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para fork() e sleep()
#include <sys/types.h> // Para pid_t

int main() {
    // pela minha pesquisa, pid deveria ser do tipo pid_t (em <sys/types.h>), não int
    int pid;
    
    pid = fork(); // Cria processo filho
    // No processo pai, retorna o PID do processo filho criado
    // No processo filho, retorna 0
    // Erro: retorna -1 (e nenhum processo é criado)


    if (pid < 0) {
        printf("Erro na criação do filho\n");
        return 1;
    }
    
    if (pid == 0) {
        // se estamos no processo filho
        // imprimir PID do filho e valor da variável pid
        printf("Processo Filho: PID = %d, valor do PID (var) = %d.\n", getpid(), pid);

        // Loop (5x) do processo filho
        for (int i = 0; i < 5; i++) {
            printf("Mensagem %d - processo filho\n", i + 1);
            sleep(1); // Espera por 1 segundo
        }
    } else {
        // se pid > 0, imprimimos PID do processo pai
        printf("Processo Pai: PID = %d, PID do Filho = %d\n", getpid(), pid);
    }

    return 0;
}
