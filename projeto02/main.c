/* PROJETO 02 - SIMULADOR DE PAGINAÇÃO
   Sistemas Operacionais - 04P11

   Alan Meniuk Gleizer – 10416804
   Caio Vinicius Corsini Filho – 10342005

   >> ATENÇÃO: este programa foi desenvolvido para execução em Linux e testado no Ubuntu 24.04.1 LTS

   Orientações de compilação e execução
   Para compilar: gcc *.c -o simulador

   Para executar:
    Uso: ./simuladorOS.exe [opção]
    Opções (selecione uma):
      -a <config.txt>  Especificar um arquivo de configuração.
         NÃO digite "<" ou ">".
         mais infos no arquivo config.txt
      -m               Entrada manual dos parâmetros.
      -help            Mostrar essa mensagem de ajuda
      <vazio>          Uso dos valores padrão.

   Durante a execução, pressione Ctrl+C para pausar a execução.
   Uma execução pausada pode ser retomada pressionando Enter.
   Pressione Ctrl+\ para encerrar o programa durante a execução.
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>   // para rand
#include <unistd.h> // para leitura de argc e argv
#include <locale.h>
#include <signal.h> // para SIGINT
#include "estruturasMemoria.h"
#include "inicializacoes.h"
#include "utils.h"
#include "simuladorOS.h"

// variaveis globais para gerenciamento do SIGINT
volatile bool pausado = false;
volatile bool sair = false;

void gerenciarSigint(int sig)
{
    if (!pausado)
    {
        printf("\nPrograma pausado.\nPressione Enter para retomar ou Ctrl+\\ para sair...\n");
        pausado = true;

        // programa estava pausando e retomando imediatamente...
        // limpar buffer
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
    }
}

void gerenciarSigquit(int sig)
{
    printf("\nEncerrando execução...\n");
    sair = true;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // -- CONSTANTES DO PROGRAMA
    // valores padrão que serão sobrescritos pelo arquivo de configuração ou pela entrada manual
    int TAMANHO_FRAME = 4096;
    int TAMANHO_PAGINA = 4096;
    int NUM_FRAMES = 5;
    int NUM_PAGINAS = 25;     // isso seria a mem virtual (swap), que ainda não está implementada
    int NUM_PAGINAS_PROC = 8; // tamanho do espaço de endereçamento
    int QTD_PROCESSOS = 5;
    int QTD_ACESSOS = 20;
    int DELAY_MEM_SEC = 10; // delay para acesso a memoria secundaria (em microsegundos)
    char NOME_LOG[50] = "logSimuladorPadrao.txt";
    char NOME_CONFIG[50] = "config.txt";

    // variaveis para controle do SIGINT
    signal(SIGINT, gerenciarSigint);
    signal(SIGQUIT, gerenciarSigquit);

    // estatísticas
    int totalPageFaults = 0;
    int totalPageHits = 0;

    // LEITURA DE ARGC E ARGV
    int opt;
    bool opcaoA = false;
    bool opcaoM = false;

    if (argc == 1)
    {
        // se programa foi rodado sem nenhum argumento
        printf("Você não informou argumentos para a execução do programa.\n");
        printf("O simulador será carregado com os valores padrão de configuração.\n");
        printf("Execute o programa com -help para ver as opções.\n");
    }

    while ((opt = getopt(argc, argv, "a:mhelp")) != -1)
    {
        switch (opt)
        {
        case 'a':
            opcaoA = true;
            if (optarg == NULL)
            {
                printf("AVISO: Você selecionou opção para ler arquivo de configuração, mas não informou o nome de um arquivo.\n");
                printf("AVISO: O arquivo padrão \"config.txt\" será utilizado.\n");
                printf("Execute o programa com -help para ver as opções.\n");
            }
            else
            {
                strncpy(NOME_CONFIG, optarg, sizeof(NOME_CONFIG) - 1);
                NOME_CONFIG[sizeof(NOME_CONFIG) - 1] = '\0'; // null twerminator
                printf("Opção -a selecionada com arquivo de configuração: %s\n", NOME_CONFIG);
            }
            break;
        case 'm':
            if (opcaoA)
            {
                printf("Você não pode selecionar leitura de arquivo e leitura manual ao mesmo tempo.\n");
                printf("Execute o programa com -help para ver as opções.\n");
                printf("Terminando execução.\n");
                return 1;
            }
            opcaoM = true;
            printf("Você selecionou leitura manual dos valores.\n");
            break;
        case 'h': // help e variacoes
        case '?':
            imprimirHelp();
            return 0;
        default:
            imprimirHelp();
            return 1;
        }
    }

    if (optind < argc)
    {
        printf("Erro: Argumento desconhecido \"%s\".\n", argv[optind]);
        printf("Use -help para ver as opções válidas.\n");
        return 1;
    }

    if (opcaoA)
    {
        // procura por arquivo de config
        bool arquivoConfigEncontrado = carregarConfig(NOME_CONFIG, &TAMANHO_FRAME, &TAMANHO_PAGINA, &NUM_FRAMES, &NUM_PAGINAS, &NUM_PAGINAS_PROC, &QTD_PROCESSOS, &QTD_ACESSOS, &DELAY_MEM_SEC, NOME_LOG);
        if (!arquivoConfigEncontrado)
        {
            printf("Erro na leitura do arquivo de configuração.\nVerifique o nome e a estrutura do arquivo.\n");
            return 1;
        }
    }

    if (opcaoM)
    {
        // Input e validação de TAMANHO_FRAME
        do
        {
            printf("Informe a o tamanho do frame e da página (em bytes, >0): ");
            scanf("%d", &TAMANHO_FRAME);
            if (TAMANHO_FRAME <= 0)
            {
                printf("Valor inválido. Tente novamente.\n");
            }
        } while (TAMANHO_FRAME <= 0);

        TAMANHO_PAGINA = TAMANHO_FRAME; // precisam ser iguais!

        // Input e validação de NUM_FRAMES
        do
        {
            printf("Informe a quantidade de frames na memória principal (>0): ");
            scanf("%d", &NUM_FRAMES);
            if (NUM_FRAMES <= 0)
            {
                printf("Valor inválido. Tente novamente.\n");
            }
        } while (NUM_FRAMES <= 0);

        /*
        // Input e validação de NUM_PAGINAS
        do
        {
            printf("Informe a quantidade de páginas na memória virtual (>0): ");
            scanf("%d", &NUM_PAGINAS);
            if (NUM_PAGINAS <= 0)
            {
                printf("Valor inválido. Tente novamente.\n");
            }
        } while (NUM_PAGINAS <= 0);
        */

        // Input e validação de NUM_PAGINAS_PROC
        do
        {
            printf("Informe a quantidade de páginas no espaço de endereçamento dos processos (>0): ");
            scanf("%d", &NUM_PAGINAS_PROC);
            if (NUM_PAGINAS_PROC <= 0)
            {
                printf("Valor inválido. Tente novamente.\n");
            }
        } while (NUM_PAGINAS_PROC <= 0);

        // Input e validação de QTD_PROCESSOS
        do
        {
            printf("Informe a quantidade de processos (>0): ");
            scanf("%d", &QTD_PROCESSOS);
            if (QTD_PROCESSOS <= 0)
            {
                printf("Valor inválido. Tente novamente.\n");
            }
        } while (QTD_PROCESSOS <= 0);

        // Input e validação de QTD_ACESSOS
        do
        {
            printf("Informe a quantidade de acessos (>0): ");
            scanf("%d", &QTD_ACESSOS);
            if (QTD_ACESSOS <= 0)
            {
                printf("Valor inválido. Tente novamente.\n");
            }
        } while (QTD_ACESSOS <= 0);

        // Input e validação de DELAY_MEM_SEC
        do
        {
            printf("Informe o tempo de acesso à memória secundária (em microsegundos, >0): ");
            scanf("%d", &DELAY_MEM_SEC);
            if (DELAY_MEM_SEC <= 0)
            {
                printf("Valor inválido. Tente novamente.\n");
            }
        } while (DELAY_MEM_SEC <= 0);

        // Input e validação de NOME_LOG
        do
        {
            printf("Informe o nome do arquivo de log (string .txt): ");
            scanf("%s", NOME_LOG);
            if (strlen(NOME_LOG) == 0)
            {
                printf("Nome de arquivo inválido. Tente novamente.\n");
            }
        } while (strlen(NOME_LOG) == 0);
    }

    // abertura do LOG
    FILE *arquivoLog = fopen(NOME_LOG, "w"); // arquivo sera criado se não existir, e SOBREESCRITO se existir
    if (arquivoLog == NULL)
    {
        perror("ERRO: não foi possivel abrir o arquivo de log.");
        return 1;
    }

    fprintf(arquivoLog, ">> INICIANDO SIMULADOR <<\n\n");

    printf("\nCarregamento bem-sucedido.\nValores definidos:\n");
    printf("TAMANHO_FRAME = %d\n", TAMANHO_FRAME);
    printf("TAMANHO_PAGINA = %d\n", TAMANHO_PAGINA);
    printf("NUM_FRAMES = %d\n", NUM_FRAMES);
    printf("NUM_PAGINAS = %d\n", NUM_PAGINAS);
    printf("NUM_PAGINAS_PROC = %d\n", NUM_PAGINAS_PROC);
    printf("QTD_PROCESSOS = %d\n", QTD_PROCESSOS);
    printf("QTD_ACESSOS = %d\n", QTD_ACESSOS);
    printf("DELAY_MEM_SEC = %d\n", DELAY_MEM_SEC);
    printf("NOME_LOG = %s\n", NOME_LOG);

    fprintf(arquivoLog, "TAMANHO_FRAME: %d\n", TAMANHO_FRAME);
    fprintf(arquivoLog, "TAMANHO_PAGINA: %d\n", TAMANHO_PAGINA);
    fprintf(arquivoLog, "NUM_FRAMES: %d\n", NUM_FRAMES);
    fprintf(arquivoLog, "NUM_PAGINAS: %d\n", NUM_PAGINAS);
    fprintf(arquivoLog, "NUM_PAGINAS_PROC: %d\n", NUM_PAGINAS_PROC);
    fprintf(arquivoLog, "QTD_PROCESSOS: %d\n", QTD_PROCESSOS);
    fprintf(arquivoLog, "QTD_ACESSOS: %d\n", QTD_ACESSOS);
    fprintf(arquivoLog, "DELAY_MEM_SEC: %d\n", DELAY_MEM_SEC);
    fprintf(arquivoLog, "NOME_LOG: %s\n\n", NOME_LOG);

    fprintf(arquivoLog, ">> INICIANDO SIMULAÇÃO <<\n\n");

    // seed para números aleatórios
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
    // for (int i = 0; i < QTD_ACESSOS; i++)
    int i = 0;
    while (!sair && i < QTD_ACESSOS)
    {
        if (pausado)
        {
            char input;
            // esperar pelo Enter do usuário ou Q para sair
            do
            {
                input = getchar();
            } while (input != '\n');

            pausado = false;
            printf("Retomando execução...\n");
        }

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
            totalPageHits++;
            fprintf(arquivoLog, "HIT: página %d do processo PID=%d já está na memória, no frame %d.\n", paginaAtual, processos[procAtual].pid, frameAtual);
            fprintf(arquivoLog, "  Acesso realizado. Dados do frame: %s\n", memoriaFisica[frameAtual].dados);
        }
        else
        {
            // se não está, precisamos alocar...
            totalPageFaults++;
            fprintf(arquivoLog, "PAGE FAULT: página %d do processo PID=%d NÃO está na memória.\n", paginaAtual, processos[procAtual].pid);
            pausa(DELAY_MEM_SEC); // pausa para simular acesso a memoria secundaria
            fprintf(arquivoLog, "O acesso à memória secundária levou %d microsegundos\n", DELAY_MEM_SEC);
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
                fprintf(arquivoLog, "O frame mais antigo na memória é o frameID: %d, no índice %d (posição %d).\n", frameIDSeraDesalocado, indiceFrameSeraDesalocado, indiceFrameSeraDesalocado+1);
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
        i++; // incrementar o contador geral
    }

    fprintf(arquivoLog, ">> ESTATÍSTICAS <<\n");
    fprintf(arquivoLog, "Total de acessos: %d\n", QTD_ACESSOS);
    fprintf(arquivoLog, "Total de page hits: %d\n", totalPageHits);
    fprintf(arquivoLog, "Total de page faults: %d\n", totalPageFaults);
    if (QTD_ACESSOS != totalPageFaults + totalPageHits)
    {
        fprintf(arquivoLog, "ATENÇÃO: alguns acessos não foram contabilizados.\n");
    }
    fprintf(arquivoLog, "\n");
    fprintf(arquivoLog, "%% de page hits: %.2f%%\n", (float)totalPageHits / QTD_ACESSOS * 100);
    fprintf(arquivoLog, "%% de page faults: %.2f%%\n", (float)totalPageFaults / QTD_ACESSOS * 100);
    fprintf(arquivoLog, "\n");
    fprintf(arquivoLog, "Tempo total gasto acessando a memória secundária: %d microsegundos\n", totalPageFaults * DELAY_MEM_SEC);

    // LIBERAÇÃO DA MEMÓRIA
    fclose(arquivoLog);
    for (int i = 0; i < QTD_PROCESSOS; i++)
    {
        liberarMemoriaProcesso(&processos[i], NUM_PAGINAS_PROC);
    }

    liberarMemoriaFisica(memoriaFisica, NUM_FRAMES);

    return 0;
}