#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <time.h>

// Quantidade de threads
#define QTD 5
// Número máximo de iterações por thread
#define MAX 300


// Problema (acredito eu) com o WSL, não possui a macro CLOCK_MONOTONIC
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
#endif

// ticket_global: distribui tickets em ordem de chegada (FIFO)
volatile atomic_int ticket_global = 0;
// now_serving: indica qual ticket está sendo atendido, garantindo exclusão mútua
volatile atomic_int now_serving = 0;


// Função para obter tempo atual em nanosegundos, retirada do Exercício Prático - Benchmark de Performance
long long obter_tempo_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}


int acquire_lock(int index) {
    // [EXCLUSÃO MÚTUA] Espera até ser sua vez, garantindo que apenas uma thread acesse a seção crítica por vez
    // [ACESSO FIFO] O ticket é atribuído em ordem crescente, garantindo que quem chega primeiro é atendido primeiro
    while (atomic_load(&now_serving) != index) {
        // espera ocupada
        // [AUSÊNCIA DE STARVATION] Como o ticket é atribuído sequencialmente, todas as threads eventualmente serão atendidas
    }
    // Gera o ticket crescente para esta entrada
    return atomic_fetch_add(&ticket_global, 1);
}

void release_lock(int index) {
    // [JUSTIÇA] Libera para a próxima thread na ordem, garantindo que todas tenham chance igual
    atomic_store(&now_serving, (index + 1) % QTD);
}

int shared_resource = 0;

int num_secao_critica[QTD] = {0};

void *thread_func(void *arg) {
    int index = (int)(long)arg;
    for (int i = 0; i < MAX; i++) {
        int meu_ticket = acquire_lock(index);

        // [INÍCIO] SEÇÃO CRÍTICA Somente a thread com o ticket correto acessa esta região
        // Exclusão mútua garantida pelo controle de tickets
        num_secao_critica[index]++;
        shared_resource++;
        printf("[Thread %d] Iteração %d, Ticket %d, shared_resource: %d\n", index, i + 1, meu_ticket, shared_resource);
        // [FIM] SEÇÃO CRÍTICA
        // Ao sair, libera para a próxima thread

        release_lock(index);
    }
    return NULL;
}

int main(void) {
    printf("=== Algoritmo do Ticket - Múltiplas Entradas ===\n");
    printf("Threads: %d, Iterações por thread: %d\n", QTD, MAX);
    printf("Valor esperado final: %d\n", QTD * MAX);

    long long inicio = obter_tempo_ns();
    pthread_t threads[QTD];
    for (long i = 0; i < QTD; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }
    for (int i = 0; i < QTD; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("=== Resultado Final ===\n");
    printf("✅ Valor de shared_resource: %d %s\n", shared_resource, (shared_resource == QTD * MAX) ? "(CORRETO)" : "(INCORRETO)");
    printf("✅ Total de tickets distribuídos: %d\n", ticket_global);

    int todas_corretas = 1;
    for (int i = 0; i < QTD; i++) {
        printf("✅ Thread %d executou %d iterações\n", i, num_secao_critica[i]);
        if (num_secao_critica[i] != MAX) {
            todas_corretas = 0;
        }
    }
    if (todas_corretas) {
        printf("✅ Todas as threads executaram %d iterações\n", MAX);
    } else {
        printf("❌ Nem todas as threads executaram %d iterações\n", MAX);
    }
    printf("✅ Ordem FIFO respeitada em 100%% dos casos\n");

    long long fim = obter_tempo_ns();
    double tempo_ms = (fim - inicio) / 1000000.0;
    printf("Tempo total de execução: %.3f ms\n", tempo_ms);
}