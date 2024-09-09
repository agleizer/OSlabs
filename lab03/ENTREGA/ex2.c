/*
EXERCÍCIO 2. Modifique o programa anterior para que o processo pai use wait() para aguardar que o processo filho termine antes de continuar sua execução.
Após o término do filho, o pai deve imprimir uma mensagem informando que o filho terminou.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para fork() e sleep()
#include <sys/types.h> // Para pid_t, não usado
#include <sys/wait.h> // Para wait()

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

        wait(NULL);
        // Espera o término do processo filho
        // fonte: https://stackoverflow.com/questions/42426816/in-what-way-does-waitnull-work-exactly-in-c
        printf("Estamos no processo pai!\nO processo filho já terminou!\n");
    }

    return 0;
}
