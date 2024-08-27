/*
Exercício 2: Contar Linhas em um Arquivo
Escreva um programa em C que:
    1. Abra um arquivo de texto (lab2_ex2.txt) e leia seu conteúdo.
    2. Conte quantas linhas existem no arquivo.
    3. Exiba o número total de linhas na tela.
Requisitos:
    • Utilize um buffer de 128 bytes para ler o arquivo em partes.
    • A contagem de linhas deve ser feita identificando os caracteres de quebra de linha (\n).
    • O loop deve continuar até que todo o arquivo tenha sido processado.
*/

#include <stdio.h>   // printf, perror
#include <unistd.h>  // read, write e close
#include <fcntl.h>   // open + flags
#include <stdlib.h> 

#define TAM_BUFFER 128
#define MULTIPLICADOR_BUFFER 100

int main() {
    
    // definicao das variáveis
    const char *arquivoEntrada = "lab2_ex2.txt"; // nome do arquivo esperado
    int fdEntrada; // file descriptor para armazendar retorno da função open
    ssize_t bytesLidos; // para armazenar retorno da função read
    char buffer[TAM_BUFFER]; // buffer para armazenar conteúdo lido
    char conteudo[TAM_BUFFER * MULTIPLICADOR_BUFFER];

    // abertura do arquivo e verificacao
    // int fd = open(const char *<caminho do arquivo>, int <modo de abertura>, mode_t <permissões do arq.> (opcional));
    // open retorna um descritor de arquivo (file descriptor) do tipo int
    fdEntrada = open(arquivoEntrada, O_RDONLY);
    if (fdEntrada == -1) {
        perror("Erro na abertura do arquivo!\n");
        return EXIT_FAILURE;
    } else printf("Arquivo aberto com sucesso!\n");

    // leitura do arquivo e armazenamento em um array de char
    int indiceConteudo = 0; // utlimo indice escrito em conteudo para poder colocar \0
    int qtdLinhas = 0; 

    while ((bytesLidos = read(fdEntrada, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesLidos; i++) {
            if (buffer[i] == '\n') qtdLinhas++;
            conteudo[indiceConteudo++] = buffer[i];
            
        }
    }

    // inclusão do null terminator
    conteudo[indiceConteudo] = '\0';


    // impressao do conteudo
    for (int i = 0; i < indiceConteudo; i++) {
        printf("%c", conteudo[i]);
    }

    printf("\nQuantidade de linhas = %d\n", qtdLinhas);

    close(fdEntrada);


}