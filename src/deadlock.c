#include "account.h"
#include <windows.h>


int transfer(int from_id, int to_id, double amount) {
    if (from_id < 0 || from_id >= MAX_ACCOUNTS) return -1;
    if (to_id < 0 || to_id >= MAX_ACCOUNTS) return -1;
    if (from_id == to_id) return -1;
    
    Account* first  = (from_id < to_id) ? &accounts[from_id] 
                                         : &accounts[to_id];
    Account* second = (from_id < to_id) ? &accounts[to_id]   
                                         : &accounts[from_id];
    
    Account* from = &accounts[from_id];
    Account* to   = &accounts[to_id];
    
    pthread_mutex_lock(&first->lock);
    pthread_mutex_lock(&second->lock);
    
    if (from->balance < amount) {
        printf("[TRANSFER FAILED] %s → %s: Rs.%.2f | Insufficient funds\n",
               from->name, to->name, amount);
        pthread_mutex_unlock(&second->lock);
        pthread_mutex_unlock(&first->lock);
        return -1;
    }
    
    from->balance -= amount;
    to->balance   += amount;
    from->transaction_count++;
    to->transaction_count++;
    
    printf("[TRANSFER] %s → %s: Rs.%.2f | From: Rs.%.2f | To: Rs.%.2f\n",
           from->name, to->name, amount,
           from->balance, to->balance);
    
    pthread_mutex_unlock(&second->lock);
    pthread_mutex_unlock(&first->lock);
    return 0;
}
typedef struct {
    int from;
    int to;
    double amount;
} TransferArgs;

void* transfer_thread(void* arg) {
    TransferArgs* args = (TransferArgs*)arg;
    
    for (int i = 0; i < 5; i++) {
        transfer(args->from, args->to, args->amount);
        Sleep(100);
    }
    return NULL;
}

void test_deadlock() {
    printf("\n=== DEADLOCK PREVENTION TEST ===\n");
    printf("Two threads transferring in OPPOSITE directions simultaneously\n\n");
    
    printf("Initial balances:\n");
    printf("Account 0 (Isha):  Rs.%.2f\n", accounts[0].balance);
    printf("Account 1 (Ravi):  Rs.%.2f\n\n", accounts[1].balance);
    
    pthread_t t1, t2;
    TransferArgs args1 = {0, 1, 200.0};
    TransferArgs args2 = {1, 0, 150.0};
    
    pthread_create(&t1, NULL, transfer_thread, &args1);
    pthread_create(&t2, NULL, transfer_thread, &args2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("\nFinal balances:\n");
    printf("Account 0 (Isha):  Rs.%.2f\n", accounts[0].balance);
    printf("Account 1 (Ravi):  Rs.%.2f\n", accounts[1].balance);
    printf("\n=== DEADLOCK PREVENTION TEST DONE ===\n");
}