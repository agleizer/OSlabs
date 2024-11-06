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
//#include <windows.h>

//void pausa(int milisegundos) {Sleep(milisegundos);}

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

    /*
    // esse é o jeito correto, que aparece em todos os foruns..
    // mas, como nossos endereços são indices dos arrays, não me parece fazer sentido
    int pagina_id = endereco_virtual / TAMANHO_PAGINA;
    int offset = endereco_virtual % TAMANHO_PAGINA;
    */

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

void main(){





/*
int tam_memoriaFisica = NUM_FRAMES*sizeof(frame);
int tam_memoriaVirtual = NUM_PAGINAS*sizeof(pagina);
int tam_tabelaPaginas = NUM_PAGINAS*sizeof(linhaTabelaDePaginas);
*/
// pagina memoriaVirtual[NUM_PAGINAS]; // não vamos usar..

// Configura o terminal do powershell para UTF-8 no Windows.. remover na entrega do projeto

// Create six individual 'pagina' instances


#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

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
    liberarMemoriaProcesso(&proc1);
    liberarMemoriaFisica(memoriaFisica);
}