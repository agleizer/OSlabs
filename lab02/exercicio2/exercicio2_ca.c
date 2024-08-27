// Lab 2 - Alan Gleizer - Caio Corsini

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 128

int main() {

    // Nome do arquivo de entrada
    const char *arquivo_entrada = "lab2_ex2.txt";
    //const char *arquivo_entrada = "exemplo.txt";

    // Descritores de arquivos
    int fd_in; // Variavel para o arquivo em si
    char buffer[BUFFER_SIZE]; // Variavel para armazenar cada linha do arquivo lido
    ssize_t bytes_read;

    int contagem = 1;

    // Abrindo o arquivo de entrada no modo de leitura
    fd_in = open(arquivo_entrada, O_RDONLY); //O_RDONLY = read only
    if (fd_in == -1) {
        perror("Não foi possível abrir o arquivo de entrada");
        return EXIT_FAILURE;
    } else {
        while ((bytes_read = read(fd_in, buffer, sizeof(buffer)-1)) > 0) {
            for(int i=0; i<sizeof(buffer);i++){
                if(buffer[i] == '\n') contagem++;
            }
        }
    }
    printf("Numero de linhas: %d", contagem);
    return EXIT_SUCCESS;
}