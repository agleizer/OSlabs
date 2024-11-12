#include <stdlib.h>
#include <time.h> // para rand
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include "estruturasMemoria.h"

int gerarNumeroAleatorio(int max)
{
    return rand() % max;
}

void imprimirHelp()
{
    printf("Uso: ./simuladorOS.exe [opção]\n");
    printf("Opções (selecione uma):\n");
    printf("  -a <config.txt>  Especificar um arquivo de configuração.\n");
    printf("     NÃO digite \"<\" ou \">\".\n");
    printf("     mais infos no arquivo config.txt\n");
    printf("  -m               Entrada manual dos parâmetros.\n");
    printf("  -help            Mostrar essa mensagem de ajuda\n");
    printf("  <vazio>          Uso dos valores padrão.\n");
    printf("\n");
    printf("Durante a execução, pressione Ctrl+C para pausar a execução.\n");
    printf("Uma execução pausada pode ser retomada pressionando Enter.\n");
    printf("Pressione Ctrl+\\ para encerrar o programa durante a execução.\n");
}

bool carregarConfig(char *NOME_CONFIG, int *TAMANHO_FRAME, int *TAMANHO_PAGINA, int *NUM_FRAMES,
                    int *NUM_PAGINAS, int *NUM_PAGINAS_PROC, int *QTD_PROCESSOS, int *QTD_ACESSOS,
                    int *DELAY_MEM_SEC, char *NOME_LOG)
{
    FILE *file = fopen(NOME_CONFIG, "r");
    if (file == NULL)
    {
        perror("Erro: Não foi possível abrir o arquivo de configuração.\n");
        return false;
    }

    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        // Ignorar comentários e linhas vazias
        if (line[0] == '\n' || strncmp(line, "//", 2) == 0)
        {
            continue;
        }

        // Tokenizar linha -> chave e valor
        char *key = strtok(line, " \t\n");
        char *value_str = strtok(NULL, " \t\n");

        if (!key || !value_str)
        {
            printf("Aviso: Linha de configuração malformada ignorada.\n");
            continue;
        }

        int value = atoi(value_str); // Converter valor para inteiro

        // verificar qual a variável lida
        if (strcmp(key, "TAMANHO_FRAME") == 0)
        {
            *TAMANHO_FRAME = value;
            // printf("Alterando TAMANHO_FRAME para %d\n", *TAMANHO_FRAME);
        }
        else if (strcmp(key, "TAMANHO_PAGINA") == 0)
        {
            *TAMANHO_PAGINA = value;
            // printf("Alterando TAMANHO_PAGINA para %d\n", *TAMANHO_PAGINA);
        }
        else if (strcmp(key, "NUM_FRAMES") == 0)
        {
            *NUM_FRAMES = value;
            // printf("Alterando NUM_FRAMES para %d\n", *NUM_FRAMES);
        }
        else if (strcmp(key, "NUM_PAGINAS") == 0)
        {
            *NUM_PAGINAS = value;
            // printf("Alterando NUM_PAGINAS para %d\n", *NUM_PAGINAS);
        }
        else if (strcmp(key, "NUM_PAGINAS_PROC") == 0)
        {
            *NUM_PAGINAS_PROC = value;
            // printf("Alterando NUM_PAGINAS_PROC para %d\n", *NUM_PAGINAS_PROC);
        }
        else if (strcmp(key, "QTD_PROCESSOS") == 0)
        {
            *QTD_PROCESSOS = value;
            // printf("Alterando QTD_PROCESSOS para %d\n", *QTD_PROCESSOS);
        }
        else if (strcmp(key, "QTD_ACESSOS") == 0)
        {
            *QTD_ACESSOS = value;
            // printf("Alterando QTD_ACESSOS para %d\n", *QTD_ACESSOS);
        }
        else if (strcmp(key, "DELAY_MEM_SEC") == 0)
        {
            *DELAY_MEM_SEC = value;
            // printf("Alterando DELAY_MEM_SEC para %d\n", *DELAY_MEM_SEC);
        }
        else if (strcmp(key, "NOME_LOG") == 0)
        {
            strncpy(NOME_LOG, value_str, 49);
            NOME_LOG[49] = '\0';
            // printf("Alterando NOME_LOG para %s\n", NOME_LOG);
        }
        else
        {
            printf("Aviso: Chave de configuração desconhecida ignorada: %s\n", key);
        }
    }

    fclose(file);
    return true;
}

// IMPRESSOES

#include <stdio.h>

void imprimirMemoriaFisica(frame frames[], FILE *arquivoLog, int NUM_FRAMES)
{
    fprintf(arquivoLog, "Estado da Memoria Fisica:\n");
    fprintf(arquivoLog, "Imprimindo %d frames\n", NUM_FRAMES);

    for (int i = 0; i < NUM_FRAMES; i++)
    {
        fprintf(arquivoLog, "----------\n");

        if (frames[i].ocupado)
        {
            fprintf(arquivoLog, "| P%02d-%02d |\n", frames[i].processo_id, frames[i].pagina_id);
        }
        else
        {
            fprintf(arquivoLog, "|  LIVRE   |\n");
        }
    }

    fprintf(arquivoLog, "----------\n");
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
        fprintf(arquivoLog, "Processo %02d:\n", processos[i].pid);
        //fprintf(arquivoLog, "  Tamanho do Processo: %d\n", processos[i].tamanho_processo);
        fprintf(arquivoLog, "  Numero de Enderecos: %d\n", processos[i].num_enderecos);

        // Print enderecos array, if initialized
        fprintf(arquivoLog, "  Enderecos que serão acessados: ");
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
void pausa(int microsegundos)
{
    usleep(microsegundos);
}
