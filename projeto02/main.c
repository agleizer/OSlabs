#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // para rand
#include "estruturasMemoria.h"
#include "inicializacoes.h"
#include "utils.h"
#include "simuladorOS.h"

int main()
{
// Configura o terminal do powershell para UTF-8 no Windows.. remover na entrega do projeto
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    // abertura do LOG
    FILE *arquivoLog = fopen("logSimulador.txt", "w");
    if (arquivoLog == NULL)
    {
        perror("ERRO: não foi possivel abruir o arquivo de log.");
        return 1;
    }

    fprintf(arquivoLog, ">> INICIANDO SIMULAÇÃO <<\n\n");

    // seed para desalocação aleatória
    srand(time(NULL));

    // iniciando o clock
    int relogio = 0;
    fprintf(arquivoLog, "T = %d\n", relogio++);

    fprintf(arquivoLog, "Inicializando estruturas...\n");
    // criar memoria fisica
    frame memoriaFisica[NUM_FRAMES];
    inicializarMemoFisica(memoriaFisica);
    fprintf(arquivoLog, "Memória física inicializada!\n");

    // criar processos
    processo processos[QTD_PROCESSOS];
    for (int i = 0; i < QTD_PROCESSOS; i++)
    {
        processo temp;
        int pid = inicializarProcesso(&temp, i);
        fprintf(arquivoLog, "Processo PID=%d inicializado!\n", pid);

        processos[i] = temp;
    }

    fprintf(arquivoLog, "\nProcessos inicializados: \n");
    printProcessos(processos, QTD_PROCESSOS, arquivoLog);
    fprintf(arquivoLog, "\n");
    imprimirMemoriaFisica(memoriaFisica, arquivoLog);

    // simulação dos processos tentando acessar os endereços
    fprintf(arquivoLog, "\n\n>> INICIANDO ACESSOS <<\n");
    for (int i = 0; i < QTD_PROCESSOS * NUM_PAGINAS_PROC * 2; i++)
    {
        fprintf(arquivoLog, "T = %d\n", relogio++);
        int procAtual = gerarNumeroAleatorio(QTD_PROCESSOS);
        fprintf(arquivoLog, "Processo atual = %d\n", procAtual);
        int enderecoAtual = processos[procAtual].enderecos[gerarNumeroAleatorio(NUM_PAGINAS_PROC)];
        fprintf(arquivoLog, "Endereço que será acessado: = %d\n", enderecoAtual);
        int paginaAtual = traduzirEndVirtualParaPagina(enderecoAtual);
        fprintf(arquivoLog, "Página do processo que contém o endereço = %d\n", paginaAtual);

        int frameAtual = verificarFrameDePagina(paginaAtual, &processos[procAtual]);
        // fprintf(arquivoLog, "frameAtual = %d\n", frameAtual);

        if (frameAtual != -1)
        {
            // se está na memoria, basta acessar
            fprintf(arquivoLog, "HIT: página %d do processo PID=%d já está na memória, no frame %d.\n", paginaAtual, processos[procAtual].pid, frameAtual);
            fprintf(arquivoLog, "  Acesso realizado. Dados do frame: %s\n", memoriaFisica[frameAtual].dados);
        }
        else
        {
            // se não está, precisamos alocar...
            fprintf(arquivoLog, "PAGE FAULT: página %d do processo PID=%d NÃO está na memória.\n", paginaAtual, processos[procAtual].pid);
            int indiceLivre = (int)buscarFrameLivre(memoriaFisica);

            if (indiceLivre != -1)
            {
                fprintf(arquivoLog, "Frame livre encontrado no índice %d da memória física.\n", indiceLivre);
                // se existe um frame livre, alocar lá e acessar
                int indiceFrameAlocado = alocarFrame(memoriaFisica, &processos[procAtual], paginaAtual);
                fprintf(arquivoLog, "SUCESSO: A página %d do processo PID=%d foi alocado no frame %d\n", paginaAtual, procAtual, indiceFrameAlocado);
                // fprintf(arquivoLog, "Dados alocados no frame: %s\n", memoriaFisica[frameAtual].dados);
            }
            else
            {
                // se não, precisamos desalocar um antes
                fprintf(arquivoLog, "Nenhum frame livre foi localizado!\n");
                int indiceFrameDesalocado = desalocarFrame(memoriaFisica, processos);
                fprintf(arquivoLog, "Processo de desalocação: frame %d foi liberado com sucesso.\n", indiceFrameDesalocado);
                indiceLivre = (int)buscarFrameLivre;
                fprintf(arquivoLog, "Frame livre encontrado no índice %d da memória física.\n", indiceLivre);
                int indiceFrameAlocado = alocarFrame(memoriaFisica, &processos[procAtual], paginaAtual);
                fprintf(arquivoLog, "SUCESSO: A página %d do processo PID=%d foi alocado no frame %d\n", paginaAtual, procAtual, indiceFrameAlocado);
                // fprintf(arquivoLog, "Dados alocados no frame: %s\n", memoriaFisica[frameAtual].dados);
            }
        }
        fprintf(arquivoLog, "\n");
        imprimirMemoriaFisica(memoriaFisica, arquivoLog);
        fprintf(arquivoLog, "\n\n");
    }

    /*

    for (int i = 0; i < QTD_PROCESSOS*5)
    fprintf(arquivoLog, "Alocando frames para as páginas do processo 1...\n");
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

    return 0;
}