#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TAMANHO_FRAME 4096
#define TAMANHO_PAGINA 4096
#define NUM_FRAMES 10
#define NUM_PAGINAS 25
#define NUM_PAGINAS_PROC 5 // por processo

// ---------- FUNÇÕES PARA SIMULAÇÃO DE PAUSA ----------
// PARA LINUX
#include <unistd.h>

void pausa(int milisegundos)
{
    int microsegundos = milisegundos * 1000;
    usleep(500000);
}

/*
// PARA WINDOWS
#include <windows.h>

void pausa(int milisegundos)
{
    Sleep(milisegundos);
}
*/

// ---------- ESTRUTURAS ----------
// Frame individual da mem física
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
    int processo_id;
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
    pagina *espacoEnderecamento;         //[NUM_PAGINAS_PROC]
    linhaTabelaDePaginas *tabelaPaginas; //[NUM_PAGINAS_PROC]
} processo;

// ---------- INICIALIZAÇÕES ----------
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

void inicializarTabela(linhaTabelaDePaginas linhas_tabela[])
{
    for (int i = 0; i < NUM_PAGINAS; i++)
    {
        linhas_tabela[i].end_pagina = i; // paginas são 0 a max
        linhas_tabela[i].end_frame = -1; // inicializa com -1 pois ainda não está na mem. fisica
    }
}

void inicializarProcesso(processo *proc, int pid)
{
    proc->pid = pid;
    proc->num_enderecos = NUM_PAGINAS_PROC; // numEnderecos;
    proc->enderecos = (int *)malloc(NUM_PAGINAS_PROC * sizeof(int));

    for (int i = 0; i < NUM_PAGINAS_PROC; i++)
    {
        proc->enderecos[i] = i; 
    }

    proc->tamanho_processo = NUM_PAGINAS_PROC * TAMANHO_PAGINA;
    proc->espacoEnderecamento = (pagina *)malloc(NUM_PAGINAS_PROC * sizeof(pagina));
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
int buscarFrameLivre(frame memoriaFisica[])
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        if (!memoriaFisica[i].ocupado)
        {
            return i;
        }
    }
    return -1; // nenhum frame livre
}

// alocar um frame da memória física para uma página de um processo
int alocarFrame(frame memoriaFisica[], processo *proc, int end_pagina)
{
    int indice_frame = buscarFrameLivre(memoriaFisica);
    if (indice_frame == -1)
    {
        printf("LOG: Sem frames livres na memória física.\n");
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

    return indice_frame;
}

// desalocar um frame da memória física de uma página de um processo
void desalocarFrame(frame memoriaFisica[], processo *proc, int end_pagina)
{
    int indice_frame = proc->tabelaPaginas[end_pagina].end_frame;
    if (indice_frame != -1)
    {
        // Libera o frame na memória física
        memoriaFisica[indice_frame].ocupado = false;
        memoriaFisica[indice_frame].processo_id = -1;
        memoriaFisica[indice_frame].pagina_id = -1;

        // Atualiza a tabela de páginas do processo para indicar que a página não está mais na memória física
        proc->tabelaPaginas[end_pagina].end_frame = -1;
    }
}

// traduzir endereço virtual em físico
int traduzirEndereco(int endereco_virtual, processo *proc, frame memoriaFisica[])
{

    int pagina_id = endereco_virtual; // o processo que acessar uma pagina X (indice de 0 a N do array espaço de endereçamento E na tabela de páginas)
    int indice_frame = proc->tabelaPaginas[pagina_id].end_frame;

    if (indice_frame == -1)
    {
        printf("LOG: Page fault: Página %d não está na memória física.\n", pagina_id);
        pausa(20);                                                                     // pausa para simulação do acesso ao disco
        return -1;                                                                     // Indica page fault
    }

    return indice_frame; // == endereço do frame na memória física
}

void main()
{
    frame memoriaFisica[NUM_FRAMES];
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
    free(proc1.enderecos);
    free(proc1.espacoEnderecamento);
    free(proc1.tabelaPaginas);
}