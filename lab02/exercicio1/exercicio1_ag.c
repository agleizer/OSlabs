/*
Exercício 1: Abrir e Ler um Arquivo
Escreva um programa em C que realize as seguintes operações:
    1. Abra um arquivo de texto chamado ‘lab2_ex1.txt’ em modo leitura.
    2. Verifique se o arquivo foi aberto com sucesso. Se não, exiba uma mensagem de erro e encerre o programa.
    3. Leia o conteúdo do arquivo para um buffer.
    4. Exiba o conteúdo lido na tela.
    5. Feche o arquivo após a leitura.
Requisitos:
    • O buffer deve ter um tamanho de 128 bytes.
    • Certifique-se de adicionar um caractere nulo (\0) ao final do buffer após a leitura.
*/

#include <stdio.h>   // printf, perror
#include <unistd.h>  // read, write e close
#include <fcntl.h>   // open + flags
#include <stdlib.h> 

#define TAM_BUFFER 128
#define MULTIPLICADOR_BUFFER 100

int main() {
    
    // definicao das variáveis
    const char *arquivoEntrada = "lab2_ex1.txt"; // nome do arquivo esperado
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
    int qtdWhile = 0; // para não sobrescrever o conteúdo a cada iteração do buffer 
    while ((bytesLidos = read(fdEntrada, buffer, sizeof(buffer) - 1)) > 0) {
        for (int i = 0; i < TAM_BUFFER; i++) {
            conteudo[i + qtdWhile] = buffer[i];
            indiceConteudo++;
        }
        qtdWhile = qtdWhile + TAM_BUFFER;
    }

    // inclusão do null terminator
    conteudo[indiceConteudo+1] = '\0';


    // impressao do conteudo
    for (int i = 0; i < indiceConteudo; i++) {
        printf("%c", conteudo[i]);
    }

    close(fdEntrada);


}