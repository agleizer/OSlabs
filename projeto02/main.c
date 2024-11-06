#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // para rand
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

int gerarNumeroAleatorio(int max)
{
    return rand() % max;
}

void inicializarMemoFisica(frame memoriaFisica[])
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        memoriaFisica[i].dados = (char *)malloc(TAMANHO_FRAME * sizeof(char));
        memoriaFisica[i].id = i;
        memoriaFisica[i].ocupado = false;
        memoriaFisica[i].alterado = false;
        memoriaFisica[i].pagina_id = -1;
        memoriaFisica[i].processo_id = -1;
    }
    printf("Memória física inicializada!\n");
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
        linhas_tabela[i].end_pagina = i; // paginas são 0 a max
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
        proc->enderecos[i] = gerarNumeroAleatorio(NUM_PAGINAS_PROC * TAMANHO_PAGINA);
    }

    proc->tamanho_processo = NUM_PAGINAS_PROC * TAMANHO_PAGINA;
    proc->espacoEnderecamento = (pagina *)malloc(NUM_PAGINAS_PROC * sizeof(pagina));

    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        proc->espacoEnderecamento[i].dados = (char *)malloc(TAMANHO_PAGINA * sizeof(char));
    }

    proc->tabelaPaginas = (linhaTabelaDePaginas *)malloc(NUM_PAGINAS_PROC * sizeof(linhaTabelaDePaginas));
    inicializarTabela(proc->tabelaPaginas); // Inicializa a tabela de páginas do processo
    printf("Processo PID=%d inicializado!\n", pid);
}

// ---------- FUNÇÕES DE GERENCIAMENTO DA TABELA DE PAGINAS ----------

// retorna o indice de um endereço virtual no espaço de endereçamento do processo
int buscarIndicePorEnderecoVirtual(linhaTabelaDePaginas tabelaPaginas[], int end_pagina)
{
    for (int i = 0; i < NUM_PAGINAS; i++)
    {
        if (tabelaPaginas[i].end_pagina == end_pagina)
        {
            printf("Endereço virtual %d está localizado na página %d\n", end_pagina, i);
            return i;
        }
    }
    return -1; // Não encontrado
}

// ---------- FUNÇÕES DE MAPEAMENTO ----------

int traduzirEndVirtualParaPagina(int endereco)
{
    int pagina = endereco / TAMANHO_PAGINA;
    printf("Endereço virtual %d está localizado na página %d\n", endereco, pagina);
    return pagina;
}

int verificarFrameDePagina(int indicePagina, processo *proc)
{
    int indiceFrame = proc->tabelaPaginas[indicePagina].end_frame;
    if (indiceFrame == -1)
    {
        printf("LOG: Pagina NÃO está na memória!\n");
    }
    else
    {
        printf("LOG: Página está no indíce %d da memória física.\n", indiceFrame);
    }
    return indiceFrame;
}

// procura por um frame livre na mem. física, retorna o indíce ou -1
int buscarFrameLivre(frame memoriaFisica[])
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        if (!memoriaFisica[i].ocupado)
        {
            printf("Frame livre encontrado no índice %d da memória física.\n", i);
            return i;
        }
    }
    printf("Nenhum frame livre foi localizado!\n");
    return -1; // nenhum frame livre
}

// alocar um frame da memória física para uma página de um processo
int alocarFrame(frame memoriaFisica[], processo *proc, int end_pagina)
{
    int indice_frame = buscarFrameLivre(memoriaFisica);
    if (indice_frame == -1)
    {
        printf("LOG: Sem frames livres na memória física.\n"); // TODO: alterar para fprintf ou similar
        return -1;
    }

    // Atualiza o frame na memória física
    memoriaFisica[indice_frame].ocupado = true;
    memoriaFisica[indice_frame].processo_id = proc->pid;
    memoriaFisica[indice_frame].pagina_id = end_pagina;

    // checra se a página tem dados e copia-los para o frame
    if (proc->espacoEnderecamento[end_pagina].dados != NULL)
    {
        memcpy(memoriaFisica[indice_frame].dados, proc->espacoEnderecamento[end_pagina].dados, TAMANHO_PAGINA);
    }
    else
    {
        // se não, inicializar os dados do frame, que provavelmente será usado
        memset(memoriaFisica[indice_frame].dados, 0, TAMANHO_PAGINA);
    }

    // Atualiza a tabela de páginas do processo
    proc->tabelaPaginas[end_pagina].end_pagina = end_pagina;
    proc->tabelaPaginas[end_pagina].end_frame = indice_frame;

    printf("A página %d do processo PID=%d foi alocado no frame %d\n", end_pagina, proc->pid, indice_frame);
    return indice_frame;
}

int indiceFrameParaIndicePagina(processo proc, int indiceFrame)
{
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        if (proc.tabelaPaginas[i].end_frame == indiceFrame)
        {
            printf("Processo de desalocação: o frame %d contém a página %d do processo PID=%d.\n", indiceFrame, i, proc.pid);
            return i;
        }
    }

    return -1;
}

// desalocar um frame da memória física
void desalocarFrame(frame memoriaFisica[], processo processos[])
{
    int indice_frame = gerarNumeroAleatorio(NUM_FRAMES);

    // atualizar a tabela de paginas do processo cuja pagina foi removida
    int indiceProcessoAlterado = memoriaFisica[indice_frame].processo_id;
    int indiceDaPaginaDoProcAlterado = indiceFrameParaIndicePagina(processos[indiceProcessoAlterado], indice_frame);

    processos[indiceProcessoAlterado].tabelaPaginas[indiceDaPaginaDoProcAlterado].end_frame = -1;
    printf("Processo de desalocação: frame %d foi liberado com sucesso.\n", indice_frame);

    // Libera o frame na memória física
    memoriaFisica[indice_frame].ocupado = false;
    memoriaFisica[indice_frame].processo_id = -1;
    memoriaFisica[indice_frame].pagina_id = -1;
    // TODO FREE OS DADOS
}

// traduzir endereço virtual em físico
int traduzirEndereco(int endereco_virtual, processo *proc, frame memoriaFisica[])
{
    int pagina_id = endereco_virtual; // o processo que acessar uma pagina X (indice de 0 a N do array espaço de endereçamento E na tabela de páginas)
    int indice_frame = proc->tabelaPaginas[pagina_id].end_frame;

    if (indice_frame == -1)
    {
        printf("LOG: Page fault: Página %d não está na memória física.\n", pagina_id); // TODO: alterar para fprintf ou similar
        // pausa(20);                                                                     // pausa para simulação do acesso ao disco
        return -1; // Indica page fault
    }

    return indice_frame; // == endereço do frame na memória física
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

void liberarMemoriaFisica(frame memoriaFisica[])
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        free(memoriaFisica[i].dados);
    }
}

// IMPRESSOES

#include <stdio.h>

void imprimirMemoriaFisica(frame frames[])
{
    printf("Estado da Memoria Fisica:\n");
    printf("Imprimindo %d frames\n", NUM_FRAMES);

    for (int i = 0; i < NUM_FRAMES; i++)
    {
        printf("--------\n");

        if (frames[i].ocupado)
        {
            printf("| P%d-%d |\n", frames[i].processo_id, frames[i].pagina_id);
        }
        else
        {
            printf("| Livre |\n");
        }
    }

    printf("--------\n");
}

void printArray(int arr[], int size)
{
    printf("[");
    for (int i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i < size - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

void printProcessos(processo processos[], int size)
{
    printf("Lista de Processos:\n");
    for (int i = 0; i < size; i++)
    {
        printf("Processo %d:\n", processos[i].pid);
        printf("  Tamanho do Processo: %d\n", processos[i].tamanho_processo);
        printf("  Numero de Enderecos: %d\n", processos[i].num_enderecos);

        // Print enderecos array, if initialized
        printf("  Enderecos: ");
        if (processos[i].enderecos != NULL && processos[i].num_enderecos > 0)
        {
            for (int j = 0; j < processos[i].num_enderecos; j++)
            {
                printf("%d ", processos[i].enderecos[j]);
            }
        }
        else
        {
            printf("Nenhum");
        }
        printf("\n");

        // Here you might also want to print `espacoEnderecamento` and `tabelaPaginas`,
        // but this depends on the structure of `pagina` and `linhaTabelaDePaginas`.
    }
}

void main()
{
// Configura o terminal do powershell para UTF-8 no Windows.. remover na entrega do projeto
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    srand(time(NULL)); // seed para o alatorio que será usado na desalocação

    // criar memoria fisica
    frame memoriaFisica[NUM_FRAMES];
    inicializarMemoFisica(memoriaFisica);

    // criar processos
    processo processos[QTD_PROCESSOS];
    for (int i = 0; i < QTD_PROCESSOS; i++)
    {
        processo temp;
        inicializarProcesso(&temp, i);
        processos[i] = temp;
    }

    printProcessos(processos, QTD_PROCESSOS);
    imprimirMemoriaFisica(memoriaFisica);

    // simulação dos processos tentando acessar os endereços
    for (int i = 0; i < QTD_PROCESSOS * NUM_PAGINAS_PROC * 2; i++)
    {
        int procAtual = gerarNumeroAleatorio(QTD_PROCESSOS);
        printf("procAtual = %d\n", procAtual);
        int enderecoAtual = processos[procAtual].enderecos[gerarNumeroAleatorio(NUM_PAGINAS_PROC)];
        printf("enderecoAtual = %d\n", enderecoAtual);
        int paginaAtual = traduzirEndVirtualParaPagina(enderecoAtual);
        printf("paginaAtual = %d\n", paginaAtual);

        int frameAtual = verificarFrameDePagina(paginaAtual, &processos[procAtual]);
        printf("frameAtual = %d\n", frameAtual);

        if (frameAtual != -1)
        {
            // se está na memoria, basta acessar
            printf("HIT: página %d do processo PID=%d já está na memória, no frame %d.\n", paginaAtual, processos[procAtual].pid, frameAtual);
            printf("dados: \n%s", memoriaFisica[frameAtual].dados);
        }
        else
        {
            // se não está, precisamos alocar...
            printf("PAGE FAULT: página %d do processo PID=%d NÃO está na memória.\n", paginaAtual, processos[procAtual].pid);
            int indiceLivre = (int)buscarFrameLivre(memoriaFisica);

            if (indiceLivre != -1)
            {
                // se existe um frame livre, alocar lá e acessar
                alocarFrame(memoriaFisica, &processos[procAtual], paginaAtual);
                printf("%s", memoriaFisica[frameAtual].dados);
            }
            else
            {
                // se não, precisamos desalocar um antes
                desalocarFrame(memoriaFisica, processos);
                indiceLivre = (int)buscarFrameLivre;
                alocarFrame(memoriaFisica, &processos[procAtual], paginaAtual);
                printf("%s", memoriaFisica[frameAtual].dados);
            }
        }
        imprimirMemoriaFisica(memoriaFisica);
    }

    /*

    for (int i = 0; i < QTD_PROCESSOS*5)
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
    */
}