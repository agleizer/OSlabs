#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // para rand
#include "estruturasMemoria.h"
#include "inicializacoes.h"
#include "utils.h"

void inicializarMemoFisica(frame memoriaFisica[], int NUM_FRAMES, int TAMANHO_FRAME)
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        memoriaFisica[i].dados = (char *)malloc(TAMANHO_FRAME * sizeof(char));
        memoriaFisica[i].id = -1; // inicializar IDs com -1 pois serão usado para decidir FIFO na desalocação
        memoriaFisica[i].ocupado = false;
        memoriaFisica[i].alterado = false;
        memoriaFisica[i].pagina_id = -1;
        memoriaFisica[i].processo_id = -1;
    }
}

// aqui, estaríamos simulando a mem. virtual em disco, que o Lucas disse ser opcional
void inicializarMemoVirtual(pagina memoriaVirtual[], int NUM_PAGINAS, int TAMANHO_PAGINA)
{
    for (int i = 0; i < NUM_PAGINAS; i++)
    {
        memoriaVirtual[i].dados = (char *)malloc(TAMANHO_PAGINA * sizeof(char));
        memoriaVirtual[i].id = i;
        memoriaVirtual[i].processo_id = -1;
    }
}

// Inicializa a tabela de páginas do processo com espaço de endereçamento
void inicializarTabela(linhaTabelaDePaginas linhas_tabela[], int NUM_PAGINAS_PROC)
{
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        linhas_tabela[i].end_pagina = i; // paginas são 0 a max
        linhas_tabela[i].end_frame = -1; // inicializa com -1 pois ainda não está na mem. fisica
    }
}

// inicializa processo, retorna PID
int inicializarProcesso(processo *proc, int pid, int NUM_PAGINAS_PROC, int TAMANHO_PAGINA)
{
    proc->pid = pid;
    proc->num_enderecos = NUM_PAGINAS_PROC; // numEnderecos;
    proc->enderecos = (int *)malloc(NUM_PAGINAS_PROC * sizeof(int));

    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        proc->enderecos[i] = gerarNumeroAleatorio(NUM_PAGINAS_PROC * TAMANHO_PAGINA);
    }

    proc->tamanho_processo = NUM_PAGINAS_PROC * TAMANHO_PAGINA;
    proc->espacoEnderecamento = (pagina *)malloc(NUM_PAGINAS_PROC * sizeof(pagina));

    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        proc->espacoEnderecamento[i].dados = (char *)malloc(TAMANHO_PAGINA * sizeof(char));
    }

    proc->tabelaPaginas = (linhaTabelaDePaginas *)malloc(NUM_PAGINAS_PROC * sizeof(linhaTabelaDePaginas));
    inicializarTabela(proc->tabelaPaginas, NUM_PAGINAS_PROC); // Inicializa a tabela de páginas do processo
    return pid;
}