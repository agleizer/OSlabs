/*

gcc -o proj01_threads_concorrencia proj01_threads_concorrencia.cpp -lpthread

Demonstrar o uso de threads e sincronização para garantir a 
consistência das operações de depósito e saque em uma conta bancária compartilhada.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#define INITIAL_BALANCE 1000
#define NUM_THREADS 10
#define TRANSFER_AMOUNT 50

// Estrutura para armazenar informações da conta
typedef struct {
    int balance;
    pthread_mutex_t mutex;
} BankAccount;

// Função para depositar uma quantia na conta
void deposit(BankAccount *account, int amount) {
    account->balance += amount;
    printf("Depositado: %d, Saldo Atual: %d\n", amount, account->balance);
}

// Função para sacar uma quantia da conta
void withdraw(BankAccount *account, int amount) {
    if (account->balance >= amount) {
        account->balance -= amount;
        printf("Sacado: %d, Saldo Atual: %d\n", amount, account->balance);
    } else {
        printf("Saldo Insuficiente para saque: %d\n", amount);
    }
}

// Função executada pelas threads
void* transaction_dep(void* arg) {
    BankAccount *account = (BankAccount*)arg;

    // Realiza uma série de depósitos e saques
    for (int i = 0; i < 5; i++) {
        deposit(account, TRANSFER_AMOUNT);
        sleep(3); // Simula algum tempo de processamento
   }

    return NULL;
}

void* transaction_sac(void* arg) {
    BankAccount *account = (BankAccount*)arg;

    // Realiza uma série de depósitos e saques
    for (int i = 0; i < 5; i++) {
        withdraw(account, TRANSFER_AMOUNT);
        sleep(1); // Simula algum tempo de processamento
    }

    return NULL;
}


int main() {
    pthread_t threads_dep[NUM_THREADS];
    pthread_t threads_sac[NUM_THREADS];
    BankAccount account;
    account.balance = INITIAL_BALANCE;
    pthread_mutex_init(&account.mutex, NULL);

    // Cria as threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads_dep[i], NULL, transaction_dep, &account) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads_sac[i], NULL, transaction_sac, &account) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }
    
    // Espera todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads_dep[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        sleep(1);
        }
     
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads_sac[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        }
    }

    // Destrói o mutex
    pthread_mutex_destroy(&account.mutex);

    // Imprime o saldo final
    printf("Saldo Final: %d\n", account.balance);

    return 0;
}
/*
Inclusão das Bibliotecas:

	pthread.h para trabalhar com threads e mutexes.
	unistd.h para a função sleep, que simula algum tempo de processamento.

Estrutura BankAccount:

	Armazena o saldo da conta e um mutex para sincronização.

Funções deposit e withdraw:

	Usam o mutex para garantir que apenas uma thread pode acessar e modificar o saldo da conta de cada vez. Isso evita condições de corrida e mantém a integridade dos dados.

Função transaction:

	Cada thread realiza uma série de depósitos e saques na conta.
Função main:

	Inicializa a conta bancária e o mutex.
	Cria NUM_THREADS threads, cada uma executando a função transaction.
	Espera que todas as threads terminem com pthread_join.
	Destrói o mutex e imprime o saldo final da conta.

*/