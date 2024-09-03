// Exercicio 2 - Lab 3 - Alan Gleizer & Caio Corsini
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
//cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/sistemas operacionais/lab3 processos/exercicio2.c" ~/programasOS/
//gcc -o exercicio2 exercicio2.c
//./exercicio2
int main(){
    int id_processo = fork();

    if(id_processo<0){
        printf("Erro na criacao do processo!");
        exit(1);
    }

    if(id_processo==0){
        printf("\nEste eh o filho.\nID do filho: %d\n", getpid());

        for(int i=0; i<5; i++){
            printf("Repeticao %d\n", i+1);
            sleep(1);
        }
        printf("\nFilho terminou.\n\n");
    } else {
        printf("\nEste eh o pai.\nID do pai: %d\nID do filho: %d\n\n", getpid(), id_processo);
        wait(NULL);
    }

    system("date");

    return 0;
}