#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// ---------- ESTRUTURAS ----------
// Frame individual da mem física
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    int id; // será usado para armazenar o # de alocação do frame, para implementar FIFO
    bool ocupado;
    bool alterado;   // indica se o conteúdo foi alterado desde que a página foi carregada no frame // ALAN: acho que precisa ser no frame, para saber se precisa copiar de volta pra mem virtual ou só apagar
    int processo_id; // ID do processo que está usando o frame (-1 se livre)
    int pagina_id;   // ID da página armazenada no frame (-1 se livre)
    char *dados;     // ponteiro para os dados armazenados no frame
} frame;

// página individual da mem virtual
typedef struct
{
    int id;
    int processo_id; // aaaaacho que não precisa, pois só vamos usar dentro do espaço de endereçamento, que é dentro de um processo..
    char *dados;
} pagina;

// linha indiviual da tebela de páginas
typedef struct
{
    int end_pagina; // endereço / indice da pagina do espaço de endereçamento do processo
    int end_frame;  // endereço / indice do frame na mem fisica
} linhaTabelaDePaginas;

// processo individual
typedef struct
{
    int pid;
    int *enderecos;
    int num_enderecos;
    int tamanho_processo;
    pagina *espacoEnderecamento;
    linhaTabelaDePaginas *tabelaPaginas;
} processo;

#endif