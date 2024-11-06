// ---------- ESTRUTURAS ----------
// Frame individual da mem física
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TAMANHO_FRAME 4096
#define TAMANHO_PAGINA 4096
#define NUM_FRAMES 3
#define NUM_PAGINAS 25     // isso seria a mem virtual, que provavelmente não vamos implementar
#define NUM_PAGINAS_PROC 5 // por processo

typedef struct
{
    int id;
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
    int *enderecos; // ainda não entendi o que é isso.......
    int num_enderecos;
    int tamanho_processo;
    pagina *espacoEnderecamento;         //[NUM_PAGINAS_PROC]
    linhaTabelaDePaginas *tabelaPaginas; //[NUM_PAGINAS_PROC]
} processo;

typedef struct queue {
    int primeiro;
    int ultimo;
    int contagem;
    frame dados[NUM_FRAMES];
} Fila;