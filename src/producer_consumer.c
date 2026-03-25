#include "account.h"
#include <windows.h>
void init_queue() {
    tx_queue.head = 0;
    tx_queue.tail = 0;
    tx_queue.count = 0;
    pthread_mutex_init(&tx_queue.lock, NULL);
    pthread_cond_init(&tx_queue.not_empty, NULL);
    pthread_cond_init(&tx_queue.not_full, NULL);
    printf("[QUEUE] Initialized with %d slots\n", MAX_QUEUE_SIZE);
}
//producer side
void enqueue_transaction(Transaction tx) {
    pthread_mutex_lock(&tx_queue.lock);
    
    while (tx_queue.count == MAX_QUEUE_SIZE) {
        printf("[CUSTOMER] Queue full — waiting...\n");
        pthread_cond_wait(&tx_queue.not_full, &tx_queue.lock);
    }
    
    tx_queue.queue[tx_queue.tail] = tx;
    tx_queue.tail = (tx_queue.tail + 1) % MAX_QUEUE_SIZE;
    tx_queue.count++;
    
    printf("[CUSTOMER] Added transaction %d to queue (count: %d)\n",
           tx.id, tx_queue.count);
    
    pthread_cond_signal(&tx_queue.not_empty);
    pthread_mutex_unlock(&tx_queue.lock);
}
//consumer side
Transaction dequeue_transaction() {
    pthread_mutex_lock(&tx_queue.lock);
    
    while (tx_queue.count == 0) {
        printf("[TELLER] Queue empty — waiting...\n");
        pthread_cond_wait(&tx_queue.not_empty, &tx_queue.lock);
    }
    
    Transaction tx = tx_queue.queue[tx_queue.head];
    tx_queue.head = (tx_queue.head + 1) % MAX_QUEUE_SIZE;
    tx_queue.count--;
    
    printf("[TELLER] Took transaction %d from queue (count: %d)\n",
           tx.id, tx_queue.count);
    
    pthread_cond_signal(&tx_queue.not_full);
    pthread_mutex_unlock(&tx_queue.lock);
    
    return tx;
}
void* customer_thread(void* arg) {
    int id = *(int*)arg;
    
    Transaction tx;
    tx.id = id;
    tx.type = DEPOSIT;
    tx.from_account = id % MAX_ACCOUNTS;
    tx.amount = 100.0 * (id + 1);
    tx.status = 0;
    
    enqueue_transaction(tx);
    return NULL;
}

void* teller_thread_pc(void* arg) {
    int id = *(int*)arg;
    
    Transaction tx = dequeue_transaction();
    printf("[TELLER %d] Processing: account %d, Rs.%.2f\n",
           id, tx.from_account, tx.amount);
    Sleep(500);
    printf("[TELLER %d] Done!\n", id);
    return NULL;
}

void test_producer_consumer() {
    printf("\n=== PRODUCER-CONSUMER TEST ===\n");
    printf("15 customers producing, 15 tellers consuming\n\n");
    
    init_queue();
    
    pthread_t customers[15];
    pthread_t tellers[15];
    int cust_ids[15];
    int tell_ids[15];
    
    for (int i = 0; i < 15; i++) {
        cust_ids[i] = i;
        tell_ids[i] = i;
        pthread_create(&customers[i], NULL, customer_thread, &cust_ids[i]);
        pthread_create(&tellers[i], NULL, teller_thread_pc, &tell_ids[i]);
    }
    
    for (int i = 0; i < 15; i++) {
        pthread_join(customers[i], NULL);
        pthread_join(tellers[i], NULL);
    }
    
    printf("\n=== PRODUCER-CONSUMER TEST DONE ===\n");
}