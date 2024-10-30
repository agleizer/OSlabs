#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TAMANHO_FRAME 4096
#define TAMANHO_PAGINA 4096
#define NUM_FRAMES 10
#define NUM_PAGINAS 25     // isso seria a mem virtual, que não vamos implementar
#define NUM_PAGINAS_PROC 5 // por processo

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
    int processo_id; // aaaaacho que não precisa, pois só vamos usar dentro do espaço de endereçamento, que é dentro de um processo..
    char *dados;
} pagina;

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

// linha indiviual da tebela de páginas
typedef struct
{
    int end_pagina; // endereço / indice da pagina do espaço de endereçamento do processo
    int end_frame;  // endereço / indice do frame na mem fisica
} linhaTabelaDePaginas;

// INICIALIZAÇÕES
// ALAN: tirei n da assiantura da função e substitui pelos defines
// também não acredito que preciamos criar um novo frame e copia-lo p a memoria, a mem já é um array incializado! basta acessar mem[i]
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

void inicializarProcesso(processo *proc, int pid, int numEnderecos)
{
    proc->pid = pid;
    proc->num_enderecos = numEnderecos;
    proc->enderecos = (int *)malloc(proc->num_enderecos * sizeof(int));
    proc->tamanho_processo = 2000; // NAO FACO IDEIA DO QUE VAI AQUI
    proc->espacoEnderecamento = (pagina *)malloc(NUM_PAGINAS_PROC * sizeof(pagina));
    proc->tabelaPaginas = (linhaTabelaDePaginas *)malloc(NUM_PAGINAS_PROC * sizeof(linhaTabelaDePaginas));
    inicializarTabela(proc->tabelaPaginas); // Inicializa a tabela de páginas do processo
}

// FUNÇÕES DE MAPEAMENTO
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

// repensar... não faz sentido... preciso de 1 que busque pelo end_virtual na tabela, uma que aloca, uma que desaloca...
/*
void mapearEndereco(linhaTabelaDePaginas tabelaPaginas[], int end_pagina, int end_frame)
{
    int indice = buscarIndicePorEnderecoVirtual(tabelaPaginas, end_pagina);
    if (indice == -1)
    {
        // Busca um índice livre
        for (int i = 0; i < NUM_PAGINAS; i++)
        {
            if (tabelaPaginas[i].end_frame == -1)
            { // Checa se está livre
                tabelaPaginas[i].end_pagina = end_pagina;
                tabelaPaginas[i].end_frame = end_frame;
                break;
            }
        }
    }
    else
    {
        // Atualiza o endereço físico no índice encontrado
        tabelaPaginas[indice].end_frame = end_frame;
    }
}
*/

void main()
{
    /*
    int tam_memoriaFisica = NUM_FRAMES*sizeof(frame);
    int tam_memoriaVirtual = NUM_PAGINAS*sizeof(pagina);
    int tam_tabelaPaginas = NUM_PAGINAS*sizeof(linhaTabelaDePaginas);
    */

    frame memoriaFisica[NUM_FRAMES];
    pagina memoriaVirtual[NUM_PAGINAS]; // não vamos usar..
    linhaTabelaDePaginas tabelaPaginas[NUM_PAGINAS];
}