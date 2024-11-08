#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // para rand
#include "estruturasMemoria.h"
#include "inicializacoes.h"
#include "utils.h"
#include "simuladorOS.h"

/*
TODO PARA RODAR NO LINUX
- em utils.c, alterar a função de pausa
- remover ifndef _WIN32 em main

*/

int main()
{
// Configura o terminal do powershell para UTF-8 no Windows.. remover na entrega do projeto
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    // definição das constantes de tamanho para as estruturas
    // são valores padrão que serão sobrescritos pelo arquivo de configuração
    // mantive nome em CAPS pois eram #defines e não quero reescreveer tudo
    int TAMANHO_FRAME = 4096;
    int TAMANHO_PAGINA = 4096;
    int NUM_FRAMES = 5;
    int NUM_PAGINAS = 25;     // isso seria a mem virtual, que provavelmente não vamos implementar
    int NUM_PAGINAS_PROC = 8; // por processo
    int QTD_PROCESSOS = 5;
    int DELAY_MEM_SEC = 10; // delay para acesso a memoria secundaria
    char NOME_LOG[50] = "logSimulador.txt";
    char NOME_CONFIG[50] = "config.txt";

    // procura por arquivo de config
    bool arquivoConfigEncontrado = carregarConfig(NOME_CONFIG, &TAMANHO_FRAME, &TAMANHO_PAGINA, &NUM_FRAMES, &NUM_PAGINAS, &NUM_PAGINAS_PROC, &QTD_PROCESSOS, &DELAY_MEM_SEC, NOME_LOG);

    // abertura do LOG
    FILE *arquivoLog = fopen(NOME_LOG, "w"); // arquivo sera criado se não existir, e SOBREESCRITO se existir
    if (arquivoLog == NULL)
    {
        perror("ERRO: não foi possivel abrir o arquivo de log.");
        return 1;
    }

    fprintf(arquivoLog, ">> INICIANDO SIMULADOR <<\n\n");
    if (!arquivoConfigEncontrado)
    {
        fprintf(arquivoLog, "Arquivo de configuração não encontrado!\nUtilizando valores padrão.\n");
        fprintf(arquivoLog, "Se você deseja utilizar um arquivo de configuração, é necessário que exista um \"config.txt\" no diretório do simulador.\n");
    }
    else
    {
        fprintf(arquivoLog, "Arquivo de configuração encontrado!\nUtilizando valores definidos:\n");
    }

    fprintf(arquivoLog, "TAMANHO_FRAME: %d\n", TAMANHO_FRAME);
    fprintf(arquivoLog, "TAMANHO_PAGINA: %d\n", TAMANHO_PAGINA);
    fprintf(arquivoLog, "NUM_FRAMES: %d\n", NUM_FRAMES);
    fprintf(arquivoLog, "NUM_PAGINAS: %d\n", NUM_PAGINAS);
    fprintf(arquivoLog, "NUM_PAGINAS_PROC: %d\n", NUM_PAGINAS_PROC);
    fprintf(arquivoLog, "QTD_PROCESSOS: %d\n", QTD_PROCESSOS);
    fprintf(arquivoLog, "DELAY_MEM_SEC: %d\n", DELAY_MEM_SEC);
    fprintf(arquivoLog, "NOME_LOG: %s\n\n", NOME_LOG);

    fprintf(arquivoLog, ">> INICIANDO SIMULAÇÃO <<\n\n");

    // seed para desalocação aleatória
    srand(time(NULL));

    // iniciando o clock
    int relogio = 0;
    fprintf(arquivoLog, "T = %d\n", relogio++);

    fprintf(arquivoLog, "Inicializando estruturas...\n");
    // criar memoria fisica
    frame memoriaFisica[NUM_FRAMES];
    inicializarMemoFisica(memoriaFisica, NUM_FRAMES, TAMANHO_FRAME);
    fprintf(arquivoLog, "Memória física inicializada!\n");

    // criar processos
    processo processos[QTD_PROCESSOS];
    for (int i = 0; i < QTD_PROCESSOS; i++)
    {
        processo temp;
        int pid = inicializarProcesso(&temp, i, NUM_PAGINAS_PROC, TAMANHO_PAGINA);
        fprintf(arquivoLog, "Processo PID=%d inicializado!\n", pid);

        processos[i] = temp;
    }

    fprintf(arquivoLog, "\nProcessos inicializados: \n");
    printProcessos(processos, QTD_PROCESSOS, arquivoLog);
    fprintf(arquivoLog, "\n");
    imprimirMemoriaFisica(memoriaFisica, arquivoLog, NUM_FRAMES);

    // simulação dos processos tentando acessar os endereços
    fprintf(arquivoLog, "\n\n>> INICIANDO ACESSOS <<\n");
    for (int i = 0; i < QTD_PROCESSOS * NUM_PAGINAS_PROC * 2; i++) // 2 é um número mágico só para o simulador rodar por mais tempo.. talvez seja legal deixa-lo configurável, mas o loop fica muuito grande muito rápido
    {
        fprintf(arquivoLog, "T = %d\n", relogio++);
        int procAtual = gerarNumeroAleatorio(QTD_PROCESSOS);
        fprintf(arquivoLog, "Processo atual = %d\n", procAtual);
        int enderecoAtual = processos[procAtual].enderecos[gerarNumeroAleatorio(NUM_PAGINAS_PROC)];
        fprintf(arquivoLog, "Endereço que será acessado: = %d\n", enderecoAtual);
        int paginaAtual = traduzirEndVirtualParaPagina(enderecoAtual, TAMANHO_PAGINA);
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
            pausa(DELAY_MEM_SEC); // pausa para simular acesso a memoria secundaria
            fprintf(arquivoLog, "O acesso à memória secundária levou %d ns\n", DELAY_MEM_SEC);
            int indiceLivre = (int)buscarFrameLivre(memoriaFisica, NUM_FRAMES);

            if (indiceLivre != -1)
            {
                fprintf(arquivoLog, "Frame livre encontrado no índice %d da memória física.\n", indiceLivre);
                // se existe um frame livre, alocar lá e acessar
                int indiceFrameAlocado = alocarFrame(memoriaFisica, &processos[procAtual], paginaAtual, NUM_FRAMES, TAMANHO_PAGINA, i);
                fprintf(arquivoLog, "SUCESSO: A página %d do processo PID=%d foi alocado no frame %d\n", paginaAtual, procAtual, indiceFrameAlocado);
                // fprintf(arquivoLog, "Dados alocados no frame: %s\n", memoriaFisica[frameAtual].dados);
            }
            else
            {
                // se não, precisamos desalocar um antes
                fprintf(arquivoLog, "Nenhum frame livre foi localizado! Selecionando frame para desalocar...\n");
                int indiceFrameSeraDesalocado = indiceMenorFrameIDnaMemFisica(memoriaFisica, NUM_FRAMES);
                int frameIDSeraDesalocado = memoriaFisica[indiceFrameSeraDesalocado].id;
                fprintf(arquivoLog, "O frame mais antigo na memória é o frameID: %d, no índice %d.\n", frameIDSeraDesalocado, indiceFrameSeraDesalocado);
                int indiceFrameDesalocado = desalocarFrame(memoriaFisica, processos, indiceFrameSeraDesalocado, NUM_PAGINAS_PROC, NUM_FRAMES);
                fprintf(arquivoLog, "Processo de desalocação: frame no índice %d foi liberado com sucesso.\n", indiceFrameDesalocado);
                indiceLivre = (int)buscarFrameLivre(memoriaFisica, NUM_FRAMES);
                fprintf(arquivoLog, "Frame livre encontrado no índice %d da memória física.\n", indiceLivre);
                int indiceFrameAlocado = alocarFrame(memoriaFisica, &processos[procAtual], paginaAtual, NUM_FRAMES, TAMANHO_PAGINA, i);
                fprintf(arquivoLog, "SUCESSO: A página %d do processo PID=%d foi alocado no frame %d\n", paginaAtual, procAtual, indiceFrameAlocado);
                // fprintf(arquivoLog, "Dados alocados no frame: %s\n", memoriaFisica[frameAtual].dados);
            }
        }
        fprintf(arquivoLog, "\n");
        imprimirMemoriaFisica(memoriaFisica, arquivoLog, NUM_FRAMES);
        fprintf(arquivoLog, "\n\n");
    }

    // -----------------------------------------------------------------------------------------

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