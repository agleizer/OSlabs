#include <stdio.h>
#include <stdlib.h>

int main() {
    // Nome do arquivo de entrada
    const char *arquivo_entrada = "entrada.txt";
    // Nome do arquivo de saída
    const char *arquivo_saida = "saida.txt";

    // Ponteiros para os arquivos
    FILE *file_in, *file_out;
    char ch;

    // Abrindo o arquivo de entrada no modo de leitura
    file_in = fopen(arquivo_entrada, "r");
    if (file_in == NULL) {
        perror("Não foi possível abrir o arquivo de entrada");
        return EXIT_FAILURE;
    }

    // Abrindo o arquivo de saída no modo de escrita
    file_out = fopen(arquivo_saida, "w");
    if (file_out == NULL) {
        perror("Não foi possível criar o arquivo de saída");
        fclose(file_in);
        return EXIT_FAILURE;
    }

    // Lendo o arquivo de entrada caractere por caractere e escrevendo no arquivo de saída
    while ((ch = fgetc(file_in)) != EOF) {
        fputc(ch, file_out);
    }

    // Fechando os arquivos
    fclose(file_in);
    fclose(file_out);

    printf("Arquivo '%s' criado com sucesso!\n", arquivo_saida);

    return EXIT_SUCCESS;
}