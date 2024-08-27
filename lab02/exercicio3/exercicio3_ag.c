/*
Exercício 3: Copiar o Conteúdo de um Arquivo para Outro
Escreva um programa em C que:
    1. Abra um arquivo de origem (lab2_ex3_origem.txt) em modo leitura (O_RDONLY).
    2. Abra ou crie um arquivo de destino (lab2_ex3_destino.txt) em modo escrita (o parametro do modo de abertura deve ser ‘O_WRONLY | O_CREAT | O_TRUNC’).
    3. Leia o conteúdo do arquivo de origem usando read() e escreva no arquivo de destino usando write().
    4. Feche ambos os arquivos após a operação.
Requisitos:
    • Use um buffer de 128 bytes para copiar o conteúdo.
    • Verifique se ambos os arquivos foram abertos com sucesso e trate possíveis erros.
    • Utilize um loop para ler o conteúdo do arquivo de origem e escrevê-lo no arquivo de destino até que todo o conteúdo tenha sido copiado.
*/

#include <stdio.h>   // printf, perror
#include <unistd.h>  // read, write e close
#include <fcntl.h>   // open + flags
#include <stdlib.h> 

#define TAM_BUFFER 128
#define MULTIPLICADOR_BUFFER 100

int main() {
    
    // definicao das variáveis
    const char *arquivoEntrada = "lab2_ex3_origem.txt"; // nome do arquivo esperado
    const char *arquivoSaida = "lab2_ex3_destino.txt"; // nome do arquivo de saida
    int fdEntrada; // file descriptor para armazendar retorno da função open
    int fdSaida;
    ssize_t bytesLidos; // para armazenar retorno da função read
    ssize_t bytesEscritos;
    char buffer[TAM_BUFFER]; // buffer para armazenar conteúdo lido

    // abertura do arquivo e verificacao
    // int fd = open(const char *<caminho do arquivo>, int <modo de abertura>, mode_t <permissões do arq.> (opcional));
    // open retorna um descritor de arquivo (file descriptor) do tipo int
    fdEntrada = open(arquivoEntrada, O_RDONLY);
    if (fdEntrada == -1) {
        perror("Erro na abertura do arquivo!\n");
        return EXIT_FAILURE;
    } else printf("Arquivo aberto com sucesso!\n");

    // abertura do arquivo de saída
    fdSaida = open(arquivoSaida, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdSaida == -1) {
        perror("Erro na abertura ou criação do arquivo de saída!\n");
        close(fdEntrada);
        return EXIT_FAILURE;
    } else printf("Arquivo de saída aberto com sucesso!\n");

    // leitura do arquivo e escrita na saida
    while ((bytesLidos = read(fdEntrada, buffer, sizeof(buffer))) > 0) {
        bytesEscritos = write(fdSaida, buffer, bytesLidos);
        if (bytesEscritos != bytesLidos) {
            perror("Erro na escrita do arquivo!\n");
            close(fdEntrada);
            close(fdSaida);
            return EXIT_FAILURE;
        }
        printf("Operação em execução!\n Escrevendo:\n%s\n", buffer);
    }

    printf("Operação de escrita realizada com sucesso!\n");

    printf("Fechando os arquivos.\n");
    close(fdEntrada);
    close(fdSaida);
    printf("Encerrando programa\n");

}