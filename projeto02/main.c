#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "fila.h"

// cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/sistemas operacionais/paragit2/OSlabs/projeto02/main.c" ~/projeto/ && gcc main.c -o main && ~/projeto/main
// cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/sistemas operacionais/paragit2/OSlabs/projeto02/"*.c && cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/sistemas operacionais/paragit2/OSlabs/projeto02/"*.h ~/projeto/ && ~/projeto/ && gcc ~/projeto/main.c ~/projeto/fila.c -o main && ~/projeto/main

// ---------- FUNÇÕES PARA SIMULAÇÃO DE PAUSA ----------
/* recomendação da internet parece ser usar funções diferentes no windows e linux.
vou deixar ambas implementadas, pois acredito que vamos desenvolver mais no windows
mas a entrega é em linux... */

// PARA LINUX
/*
#include <unistd.h>

void pausa(int milisegundos)
{
    int microsegundos = milisegundos * 1000;
    usleep(500000);
}
*/

// PARA WINDOWS
// #include <windows.h>

// void pausa(int milisegundos) {Sleep(milisegundos);}

// ---------- INICIALIZAÇÕES ----------
void inicializarMemoFisica(Fila *memoriaFisica)
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        frame novoFrame;
        novoFrame.dados = (char *)malloc(TAMANHO_FRAME * sizeof(char));
        novoFrame.id = i;
        novoFrame.ocupado = false;
        novoFrame.alterado = false;
        novoFrame.pagina_id = -1;
        novoFrame.processo_id = -1;

        enqueue(memoriaFisica, novoFrame); // colocar frame inicializado na memFisica
    }
}

// aqui, estaríamos simulando a mem. virtual em disco, que o Lucas disse ser opcional
void inicializarMemoVirtual(pagina memoriaVirtual[])
{
    for (int i = 0; i < NUM_PAGINAS; i++)
    {
        memoriaVirtual[i].dados = (char *)malloc(TAMANHO_PAGINA * sizeof(char));
        memoriaVirtual[i].id = i;
        memoriaVirtual[i].processo_id = -1;
    }
}

// Inicializa a tabela de páginas do processo com espaço de endereçamento
void inicializarTabela(linhaTabelaDePaginas linhas_tabela[])
{
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        linhas_tabela[i].end_pagina = i; // paginas são 0 a max // NAO SERA NECESSARIO.. O INDICE NA TABELA JÁ NOS DIZ O ENDEREÇO DA PAGINA.......
        linhas_tabela[i].end_frame = -1; // inicializa com -1 pois ainda não está na mem. fisica
    }
}

void inicializarProcesso(processo *proc, int pid /*, int numEnderecos*/)
{
    proc->pid = pid;
    proc->num_enderecos = NUM_PAGINAS_PROC; // numEnderecos;
    proc->enderecos = (int *)malloc(NUM_PAGINAS_PROC * sizeof(int));

    // os endereços que vão ser acessados são simplesmente os indices da array de espaço de endereçamento.. me parece redundante, mas vamos lá

    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        proc->enderecos[i] = i; // por isso é meio ridiculo.. essa lista é = os indices do espaço de endereçamento....
                                // poderiamos popular com inteiros de 0 até NUM_PAGINAS_PROC mas com a ordem aleatória.. seria melhor?
    }

    proc->tamanho_processo = NUM_PAGINAS_PROC * TAMANHO_PAGINA; // NAO FACO IDEIA DO QUE VAI AQUI, talvez NUM_PAGINAS_PROC * TAMANHO_PAGINA
    proc->espacoEnderecamento = (pagina *)malloc(NUM_PAGINAS_PROC * sizeof(pagina));

    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        proc->espacoEnderecamento[i].dados = (char *)malloc(TAMANHO_PAGINA * sizeof(char));
    }

    proc->tabelaPaginas = (linhaTabelaDePaginas *)malloc(NUM_PAGINAS_PROC * sizeof(linhaTabelaDePaginas));
    inicializarTabela(proc->tabelaPaginas); // Inicializa a tabela de páginas do processo
}

// ---------- FUNÇÕES DE GERENCIAMENTO DA TABELA DE PAGINAS ----------

// retorna o indice de um endereço virtual no espaço de endereçamento do processo
int buscarIndicePorEnderecoVirtual(linhaTabelaDePaginas tabelaPaginas[], int end_pagina)
{
    for (int i = 0; i < NUM_PAGINAS; i++)
    {
        if (tabelaPaginas[i].end_pagina == end_pagina)
        {
            return i;
        }
    }
    return -1; // Não encontrado
}

// ---------- FUNÇÕES DE MAPEAMENTO ----------

// procura por um frame livre na mem. física, retorna o indíce ou -1
int buscarFrameLivre(Fila *memoriaFisica)
{
    for (int i = 0; i < memoriaFisica->contagem; i++)
    {
        frame currentFrame = pegarDado(memoriaFisica, i); // acessar o frame em i
        if (!currentFrame.ocupado)
        {
            return i; // retornar o indice / endereço do frame disponivel
        }
    }
    return -1; // nenhum frame disponível
}

// alocar um frame da memória física para uma página de um processo
int alocarFrame(Fila *memoriaFisica, processo *proc, int end_pagina)
{
    int indice_frame = buscarFrameLivre(memoriaFisica);
    if (indice_frame == -1)
    {
        printf("LOG: Sem frames livres na memória física.\n");
        return -1;
    }

    // acessar o frame livre e alocar
    frame frameLivre = pegarDado(memoriaFisica, indice_frame);
    frameLivre.ocupado = true;
    frameLivre.processo_id = proc->pid;
    frameLivre.pagina_id = end_pagina;

    if (proc->espacoEnderecamento[end_pagina].dados != NULL)
    {
        memcpy(frameLivre.dados, proc->espacoEnderecamento[end_pagina].dados, TAMANHO_PAGINA);
    }
    else
    {
        memset(frameLivre.dados, 0, TAMANHO_PAGINA);
    }

    // atualizar o frame livre na fila
    memoriaFisica->dados[indice_frame] = frameLivre;

    // atualizar tabela de paginas
    proc->tabelaPaginas[end_pagina].end_pagina = end_pagina;
    proc->tabelaPaginas[end_pagina].end_frame = indice_frame;

    return indice_frame;
}

// desalocar um frame da memória física de uma página de um processo
void desalocarFrame(Fila *memoriaFisica, processo *proc, int end_pagina)
{
    int indice_frame = proc->tabelaPaginas[end_pagina].end_frame;
    if (indice_frame != -1)
    {
        // Dequeue the oldest frame, which we are freeing
        frame frameParaLiberar = dequeue(memoriaFisica);

        // Reset frame properties for reuse
        frameParaLiberar.ocupado = false;
        frameParaLiberar.processo_id = -1;
        frameParaLiberar.pagina_id = -1;
        memset(frameParaLiberar.dados, 0, TAMANHO_FRAME);

        // Enqueue the reset frame back into the queue
        enqueue(memoriaFisica, frameParaLiberar);

        // Update the page table to reflect that the page is no longer in physical memory
        proc->tabelaPaginas[end_pagina].end_frame = -1;
    }
}

// traduzir endereço virtual em físico
/*
int traduzirEndereco(int endereco_virtual, processo *proc, frame memoriaFisica[])
{
    int pagina_id = endereco_virtual; // o processo que acessar uma pagina X (indice de 0 a N do array espaço de endereçamento E na tabela de páginas)
    int indice_frame = proc->tabelaPaginas[pagina_id].end_frame;

    if (indice_frame == -1)
    {
        printf("LOG: Page fault: Página %d não está na memória física.\n", pagina_id); // TODO: alterar para fprintf ou similar
        //pausa(20);                                                                     // pausa para simulação do acesso ao disco
        return -1;                                                                     // Indica page fault
    }

    return indice_frame; // == endereço do frame na memória física
}
*/

int traduzirEndereco(int endereco_virtual, processo *proc, Fila *memoriaFisica)
{
    int pagina_id = endereco_virtual;
    int indice_frame = proc->tabelaPaginas[pagina_id].end_frame;

    if (indice_frame == -1)
    {
        printf("LOG: Page fault: Página %d não está na memória física.\n", pagina_id);
        return -1; // Indicates page fault
    }

    return indice_frame; // Frame address in physical memory
}

void liberarMemoriaProcesso(processo *proc)
{
    free(proc->enderecos);
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        free(proc->espacoEnderecamento[i].dados);
    }
    free(proc->espacoEnderecamento);
    free(proc->tabelaPaginas);
}

/*
void liberarMemoriaFisica(frame memoriaFisica[])
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        free(memoriaFisica[i].dados);
    }
}
*/

void liberarMemoriaFisica(Fila *memoriaFisica)
{
    while (!estaVazio(memoriaFisica))
    {
        frame toFree = dequeue(memoriaFisica);
        free(toFree.dados); // Free frame data
    }
    free(memoriaFisica); // Free the queue itself
}

// ------- FUNÇÕES DE IMPRESSAO
void imprimirMemoriaFisica(Fila *memoriaFisica)
{
    if (estaVazio(memoriaFisica))
    {
        printf("A memória física está vazia.\n");
        return;
    }

    printf("Conteúdo da memória física:\n");
    for (int i = 0; i < memoriaFisica->contagem; i++)
    {
        frame currentFrame = pegarDado(memoriaFisica, i);
        printf("Frame ID: %d | Ocupado: %d | Processo ID: %d | Página ID: %d\n",
               currentFrame.id, currentFrame.ocupado, currentFrame.processo_id, currentFrame.pagina_id);
    }
}

void main()
{
/*
int tam_memoriaFisica = NUM_FRAMES*sizeof(frame);
int tam_memoriaVirtual = NUM_PAGINAS*sizeof(pagina);
int tam_tabelaPaginas = NUM_PAGINAS*sizeof(linhaTabelaDePaginas);
*/
// pagina memoriaVirtual[NUM_PAGINAS]; // não vamos usar..

// Configura o terminal do powershell para UTF-8 no Windows.. remover na entrega do projeto
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    // criação da mem fisica como uma fila (para seguir FIFO)
    Fila *memoriaFisica = criarFila();

    inicializarMemoFisica(memoriaFisica);

    processo proc1;
    inicializarProcesso(&proc1, 1);

    printf("Alocando frames para as páginas do processo 1...\n");
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        alocarFrame(memoriaFisica, &proc1, i);
    }

    printf("\nTraduzindo endereços virtuais do processo 1...\n");
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        int endereco_fisico = traduzirEndereco(i, &proc1, memoriaFisica);
        if (endereco_fisico != -1)
        {
            printf("Endereço virtual %d mapeado para endereço físico %d\n", i, endereco_fisico);
        }
    }

    // Desalocação para verificação adicional
    printf("\nDesalocando frames do processo 1...\n");
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        desalocarFrame(memoriaFisica, &proc1, i);
    }

    // Liberar memória alocada
    liberarMemoriaProcesso(&proc1);
    liberarMemoriaFisica(memoriaFisica);
}