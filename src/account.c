#include"account.h"
Account accounts[MAX_ACCOUNTS];
TransactionQueue tx_queue;
void init_bank() {
    char *names[] = {
        "Isha Parveen", "Ravi Kumar", "Priya Sharma",
        "Arjun Singh", "Meera Nair", "Rohan Gupta",
        "Sneha Reddy", "Vikram Patel", "Ananya Das",
        "Karan Mehta"
    };
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        accounts[i].id = i;
        strncpy(accounts[i].name, names[i], 64);
        accounts[i].balance = 1000.0 + (i * 500.0);
        accounts[i].transaction_count = 0;
        pthread_mutex_init(&accounts[i].lock, NULL);
        pthread_rwlock_init(&accounts[i].rwlock, NULL);
        pthread_cond_init(&accounts[i].funds_available, NULL);
    }}

void print_accounts() {
    printf("\n========== BANKOS ACCOUNTS ==========\n");
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        printf("[%02d] %-20s Rs.%.2f\n",
            accounts[i].id,
            accounts[i].name,
            accounts[i].balance);
    }
    printf("=====================================\n");
}