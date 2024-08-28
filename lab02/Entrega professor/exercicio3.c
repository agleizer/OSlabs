// Lab 2 - Alan Gleizer - Caio Corsini

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 128

int main() {
    // Nome do arquivo de entrada
    const char *arquivo_entrada = "lab2_ex3_origem.txt";
    // Nome do arquivo de saída
    const char *arquivo_saida = "lab2_ex3_destino.txt";

    // Descritores de arquivos
    int fd_in; // Variavel do arquivo de entrada
    int fd_out; // Variavel do arquivo de saida
    char buffer[BUFFER_SIZE]; // Variavel que vai armazenar cada linha
    ssize_t bytes_read, bytes_written;

    // Abrindo o arquivo de entrada no modo de leitura
    fd_in = open(arquivo_entrada, O_RDONLY); // Read only
    if (fd_in == -1) {
        perror("Erro ao carregar arquivo de entrada");
        return EXIT_FAILURE;
    }

    // Abrindo o arquivo de saída no modo de escrita (e criar se não existir)
    fd_out = open(arquivo_saida, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("Erro de criacao do arquivo de saída");
        close(fd_in); // Fechando arquivo de entrada
        return EXIT_FAILURE;
    }

    printf("Arquivo '%s' criado!\n", arquivo_saida);

    // Lendo do arquivo de entrada e escrevendo no arquivo de saída
    while ((bytes_read = read(fd_in, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(fd_out, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Erro de escrita");
            // Fechando os arquivos
            close(fd_in);
            close(fd_out);
            return EXIT_FAILURE;
        }
    }

    // Fechando os arquivos
    close(fd_in);
    close(fd_out);

    if (bytes_read != -1) {
        printf("\nCopia de %s para '%s' feita com sucesso!\n", arquivo_entrada, arquivo_saida);
    }
    
    return EXIT_SUCCESS;
}