#include "account.h"
double get_balance_rw(int account_id) {
    if (account_id < 0 || account_id >= MAX_ACCOUNTS)
        return -1;
    
    Account* acc = &accounts[account_id];
    
    pthread_rwlock_rdlock(&acc->rwlock);
    double balance = acc->balance;
    pthread_rwlock_unlock(&acc->rwlock);
    
    printf("[READ] %s: Rs.%.2f\n", acc->name, balance);
    return balance;
}
int deposit_rw(int account_id, double amount) {
    if (account_id < 0 || account_id >= MAX_ACCOUNTS)
        return -1;
    
    Account* acc = &accounts[account_id];
    
    pthread_rwlock_wrlock(&acc->rwlock);
    acc->balance += amount;
    acc->transaction_count++;
    printf("[WRITE] %s: +Rs.%.2f | Balance: Rs.%.2f\n",
           acc->name, amount, acc->balance);
    pthread_rwlock_unlock(&acc->rwlock);
    
    return 0;
}

void* reader_thread(void* arg) {
    int id = *(int*)arg;
    printf("[READER %d] starting...\n", id);
    get_balance_rw(id % MAX_ACCOUNTS);
    printf("[READER %d] done\n", id);
    return NULL;
}

void* writer_thread(void* arg) {
    printf("[WRITER] starting deposit...\n");
    deposit_rw(0, 500.0);
    printf("[WRITER] deposit done\n");
    return NULL;
}

void test_rwlock() {
    printf("\n=== RWLOCK TEST ===\n");
    printf("10 readers + 1 writer running simultaneously\n\n");
    
    pthread_t readers[10];
    pthread_t writer;
    int ids[10];
    
    for (int i = 0; i < 10; i++) {
        ids[i] = i;
        pthread_create(&readers[i], NULL, reader_thread, &ids[i]);
    }
    
    pthread_create(&writer, NULL, writer_thread, NULL);
    
    for (int i = 0; i < 10; i++)
        pthread_join(readers[i], NULL);
    pthread_join(writer, NULL);
    
    printf("\n=== RWLOCK TEST DONE ===\n");
}