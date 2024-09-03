// Exercicio 3 - Lab 3 - Alan Gleizer & Caio Corsini
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
//cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/sistemas operacionais/lab3 processos/exercicio3.c" ~/programasOS/
//gcc -o exercicio3 exercicio3.c
//./exercicio3
int main(){
    int id_processo = fork();

    if(id_processo<0){
        printf("Erro na criacao do processo!");
        exit(1);
    }

    if(id_processo==0){
        printf("\nEste eh o filho.\nID do filho: %d\n", getpid());
        execl("/bin/ls", "ls", "-la", NULL);
    } else {
        printf("\nEste eh o pai.\nID do pai: %d\nID do filho: %d\n\n", getpid(), id_processo);
        wait(NULL);
    }

    system("date");

    return 0;
}