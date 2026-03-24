#include "account.h"
#include <windows.h>
void init_semaphore() {
    sem_init(&tx_queue.slots, 0, MAX_TELLERS);
    printf("[SEMAPHORE] Initialized with %d slots\n", MAX_TELLERS);
}

void* teller_thread(void* arg) {
    int id = *(int*)arg;
    
    while (1) {
        sem_wait(&tx_queue.slots);
        printf("[TELLER %d] acquired slot — processing transaction\n", id);
        
        Sleep(1000); 
        
        printf("[TELLER %d] done — releasing slot\n", id);
        sem_post(&tx_queue.slots);
        
        Sleep(1000);
    }
    return NULL;
}

void test_semaphore() {
    printf("\n=== SEMAPHORE TEST ===\n");
    printf("20 customers, only %d can transact simultaneously\n\n", 
           MAX_TELLERS);
    
    init_semaphore();
    
    pthread_t tellers[20];
    int ids[20];
    
    for (int i = 0; i < 20; i++) {
        ids[i] = i;
        pthread_create(&tellers[i], NULL, teller_thread, &ids[i]);
    }
    
    Sleep(5000);
    
    printf("\n=== SEMAPHORE TEST DONE ===\n");
}