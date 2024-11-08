#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // para rand
#include "estruturasMemoria.h"
#include "inicializacoes.h"
#include "utils.h"
#include "simuladorOS.h"

// ---------- FUNÇÕES DE GERENCIAMENTO DA TABELA DE PAGINAS ----------

// retorna o indice de um endereço virtual no espaço de endereçamento do processo
int buscarIndicePorEnderecoVirtual(linhaTabelaDePaginas tabelaPaginas[], int end_pagina, int NUM_PAGINAS)
{
    for (int i = 0; i < NUM_PAGINAS; i++)
    {
        if (tabelaPaginas[i].end_pagina == end_pagina)
        {
            // printf("Endereço virtual %d está localizado na página %d\n", end_pagina, i);
            return i;
        }
    }
    return -1; // Não encontrado
}

// ---------- FUNÇÕES DE MAPEAMENTO ----------

int traduzirEndVirtualParaPagina(int endereco, int TAMANHO_PAGINA)
{
    int pagina = endereco / TAMANHO_PAGINA;
    // printf("Endereço virtual %d está localizado na página %d\n", endereco, pagina);
    return pagina;
}

int verificarFrameDePagina(int indicePagina, processo *proc)
{
    int indiceFrame = proc->tabelaPaginas[indicePagina].end_frame;
    if (indiceFrame == -1)
    {
        // printf("LOG: Pagina NÃO está na memória!\n");
    }
    else
    {
        // printf("LOG: Página está no indíce %d da memória física.\n", indiceFrame);
    }
    return indiceFrame;
}

// procura por um frame livre na mem. física, retorna o indíce ou -1
int buscarFrameLivre(frame memoriaFisica[], int NUM_FRAMES)
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        if (!memoriaFisica[i].ocupado)
        {
            // printf("Frame livre encontrado no índice %d da memória física.\n", i);
            return i;
        }
    }
    // printf("Nenhum frame livre foi localizado!\n");
    return -1; // nenhum frame livre
}

// alocar um frame da memória física para uma página de um processo
int alocarFrame(frame memoriaFisica[], processo *proc, int end_pagina, int NUM_FRAMES, int TAMANHO_PAGINA)
{
    int indice_frame = buscarFrameLivre(memoriaFisica, NUM_FRAMES); // TODO esa busca é repetida.. poderia passar o inmdice como argumento!!!
    if (indice_frame == -1)
    {
        // printf("LOG: Sem frames livres na memória física.\n"); // TODO: alterar para fprintf ou similar
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

    // printf("A página %d do processo PID=%d foi alocado no frame %d\n", end_pagina, proc->pid, indice_frame);
    return indice_frame;
}

int indiceFrameParaIndicePagina(processo proc, int indiceFrame, int NUM_PAGINAS_PROC)
{
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        if (proc.tabelaPaginas[i].end_frame == indiceFrame)
        {
            // printf("Processo de desalocação: o frame %d contém a página %d do processo PID=%d.\n", indiceFrame, i, proc.pid);
            return i;
        }
    }

    return -1;
}

// desalocar um frame da memória física
int desalocarFrame(frame memoriaFisica[], processo processos[], int NUM_PAGINAS_PROC, int NUM_FRAMES)
{
    int indice_frame = gerarNumeroAleatorio(NUM_FRAMES);

    // atualizar a tabela de paginas do processo cuja pagina foi removida
    int indiceProcessoAlterado = memoriaFisica[indice_frame].processo_id;
    int indiceDaPaginaDoProcAlterado = indiceFrameParaIndicePagina(processos[indiceProcessoAlterado], indice_frame, NUM_PAGINAS_PROC);

    processos[indiceProcessoAlterado].tabelaPaginas[indiceDaPaginaDoProcAlterado].end_frame = -1;
    // printf("Processo de desalocação: frame %d foi liberado com sucesso.\n", indice_frame);

    // Libera o frame na memória física
    memoriaFisica[indice_frame].ocupado = false;
    memoriaFisica[indice_frame].processo_id = -1;
    memoriaFisica[indice_frame].pagina_id = -1;
    // TODO FREE OS DADOS

    return indice_frame;
}

void liberarMemoriaProcesso(processo *proc, int NUM_PAGINAS_PROC)
{
    free(proc->enderecos);
    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        free(proc->espacoEnderecamento[i].dados);
    }
    free(proc->espacoEnderecamento);
    free(proc->tabelaPaginas);
}

void liberarMemoriaFisica(frame memoriaFisica[], int NUM_FRAMES)
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        free(memoriaFisica[i].dados);
    }
}
