#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para fork(), exec() e sleep()
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

        // Substitui o processo filho pelo comando 'ls -l'
        execlp("ls", "ls", "-l", NULL);

        // erro se exec() falhar
        printf("Falha ao executar execlp\n");
        return 1;
    } else {
        // Código do processo pai
        printf("Processo Pai: PID = %d, PID do Filho = %d\n", getpid(), pid);

        // Espera o término do processo filho
        wait(NULL);
        printf("Estamos no processo pai!\nO processo filho já terminou!\n");
    }

    return 0;
}
