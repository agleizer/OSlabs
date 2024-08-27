#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main() {
    // Nome do arquivo de entrada
    const char *arquivo_entrada = "entrada.txt";
    // Nome do arquivo de saída
    const char *arquivo_saida = "saida.txt";

    // Descritores de arquivos
    int fd_in, fd_out;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    // Abrindo o arquivo de entrada no modo de leitura
    fd_in = open(arquivo_entrada, O_RDONLY);
    if (fd_in == -1) {
        perror("Não foi possível abrir o arquivo de entrada");
        return EXIT_FAILURE;
    }

    // Abrindo o arquivo de saída no modo de escrita (e criar se não existir)
    fd_out = open(arquivo_saida, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("Não foi possível criar o arquivo de saída");
        close(fd_in);
        return EXIT_FAILURE;
    }

    // Lendo do arquivo de entrada e escrevendo no arquivo de saída
    while ((bytes_read = read(fd_in, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(fd_out, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Erro ao escrever no arquivo de saída");
            close(fd_in);
            close(fd_out);
            return EXIT_FAILURE;
        }
    }

    // Verifica se ocorreu erro na leitura
    if (bytes_read == -1) {
        perror("Erro ao ler o arquivo de entrada");
    }

    // Fechando os arquivos
    close(fd_in);
    close(fd_out);

    if (bytes_read != -1) {
        printf("Arquivo '%s' criado com sucesso!\n", arquivo_saida);
    }

    return EXIT_SUCCESS;
}