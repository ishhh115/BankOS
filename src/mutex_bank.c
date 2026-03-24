#include "account.h"
int deposit(int account_id, double amount) {
    if (account_id < 0 || account_id >= MAX_ACCOUNTS)
        return -1;
    
    Account* acc = &accounts[account_id];
    
    pthread_mutex_lock(&acc->lock);
    
    acc->balance += amount;
    acc->transaction_count++;
    printf("[DEPOSIT] %s: +Rs.%.2f | Balance: Rs.%.2f\n",
           acc->name, amount, acc->balance);
    
    pthread_cond_broadcast(&acc->funds_available);
    pthread_mutex_unlock(&acc->lock);
    
    return 0;
}
int withdraw(int account_id, double amount) {
    if (account_id < 0 || account_id >= MAX_ACCOUNTS)
        return -1;
    
    Account* acc = &accounts[account_id];
    
    pthread_mutex_lock(&acc->lock);
    
    if (acc->balance < amount) {
        printf("[WITHDRAW FAILED] %s: Rs.%.2f requested | Balance: Rs.%.2f\n",
               acc->name, amount, acc->balance);
        pthread_mutex_unlock(&acc->lock);
        return -1;
    }
    
    acc->balance -= amount;
    acc->transaction_count++;
    printf("[WITHDRAW] %s: -Rs.%.2f | Balance: Rs.%.2f\n",
           acc->name, amount, acc->balance);
    
    pthread_mutex_unlock(&acc->lock);
    return 0;
}

double get_balance(int account_id) {
    if (account_id < 0 || account_id >= MAX_ACCOUNTS)
        return -1;
    
    Account* acc = &accounts[account_id];
    
    pthread_rwlock_rdlock(&acc->rwlock);
    double balance = acc->balance;
    pthread_rwlock_unlock(&acc->rwlock);
    
    return balance;
}

