#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 20
#define DEPOSITS_PER_THREAD 1000
#define DEPOSIT_AMOUNT 100.0

double shared_balance = 0.0;
pthread_mutex_t balance_mutex = PTHREAD_MUTEX_INITIALIZER;

void* deposit_broken(void* arg) {
    for (int i = 0; i < DEPOSITS_PER_THREAD; i++) {
        double temp = shared_balance;
        temp += DEPOSIT_AMOUNT;
        shared_balance = temp;
    }
    return NULL;
}

void* deposit_fixed(void* arg) {
    for (int i = 0; i < DEPOSITS_PER_THREAD; i++) {
        pthread_mutex_lock(&balance_mutex);
        shared_balance += DEPOSIT_AMOUNT;
        pthread_mutex_unlock(&balance_mutex);
    }
    return NULL;
}

void run_demo(void*(*fn)(void*), const char* label) {
    shared_balance = 0.0;
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, fn, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    double expected = NUM_THREADS * DEPOSITS_PER_THREAD * DEPOSIT_AMOUNT;
    printf("%s\n", label);
    printf("Expected: Rs.%.0f\n", expected);
    printf("Actual:   Rs.%.0f\n", shared_balance);
    if (shared_balance == expected)
        printf("Result:   CORRECT\n\n");
    else
        printf("Result:   CORRUPTED - Lost Rs.%.0f\n\n", 
               expected - shared_balance);
}

int main() {
    printf("========== BANKOS RACE CONDITION DEMO ==========\n\n");
    printf("Running %d threads, each depositing Rs.%.0f %d times\n\n",
           NUM_THREADS, DEPOSIT_AMOUNT, DEPOSITS_PER_THREAD);

    printf("--- TEST 1: WITHOUT MUTEX ---\n");
    run_demo(deposit_broken, "BROKEN (no locks):");

    printf("--- TEST 2: WITH MUTEX ---\n");
    run_demo(deposit_fixed, "FIXED (mutex protected):");

    return 0;
}