#include <stdlib.h>
#include <time.h> // para rand
#include <stdio.h>
#include "estruturasMemoria.h"

int gerarNumeroAleatorio(int max)
{
    return rand() % max;
}

// IMPRESSOES

#include <stdio.h>

void imprimirMemoriaFisica(frame frames[], FILE *arquivoLog, int NUM_FRAMES)
{
    fprintf(arquivoLog, "Estado da Memoria Fisica:\n");
    fprintf(arquivoLog, "Imprimindo %d frames\n", NUM_FRAMES);

    for (int i = 0; i < NUM_FRAMES; i++)
    {
        fprintf(arquivoLog, "--------\n");

        if (frames[i].ocupado)
        {
            fprintf(arquivoLog, "| P%d-%d |\n", frames[i].processo_id, frames[i].pagina_id);
        }
        else
        {
            fprintf(arquivoLog, "| Livre |\n");
        }
    }

    fprintf(arquivoLog, "--------\n");
}

void printArray(int arr[], int size, FILE *arquivoLog)
{
    fprintf(arquivoLog, "[");
    for (int i = 0; i < size; i++)
    {
        fprintf(arquivoLog, "%d", arr[i]);
        if (i < size - 1)
        {
            fprintf(arquivoLog, ", ");
        }
    }
    fprintf(arquivoLog, "]\n");
}

void printProcessos(processo processos[], int size, FILE *arquivoLog)
{
    fprintf(arquivoLog, "Lista de Processos:\n");
    for (int i = 0; i < size; i++)
    {
        fprintf(arquivoLog, "Processo %d:\n", processos[i].pid);
        fprintf(arquivoLog, "  Tamanho do Processo: %d\n", processos[i].tamanho_processo);
        fprintf(arquivoLog, "  Numero de Enderecos: %d\n", processos[i].num_enderecos);

        // Print enderecos array, if initialized
        fprintf(arquivoLog, "  Enderecos: ");
        if (processos[i].enderecos != NULL && processos[i].num_enderecos > 0)
        {
            for (int j = 0; j < processos[i].num_enderecos; j++)
            {
                fprintf(arquivoLog, "%d ", processos[i].enderecos[j]);
            }
        }
        else
        {
            fprintf(arquivoLog, "Nenhum");
        }
        fprintf(arquivoLog, "\n");
    }
}

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