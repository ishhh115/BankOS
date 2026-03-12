#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_ACCOUNTS 10
#define MAX_TELLERS 5
#define MAX_QUEUE_SIZE 100
#define MAX_TRANSACTIONS 1000

typedef struct {
    int id;
    char name[64];
    double balance;
    pthread_mutex_t lock;
    pthread_rwlock_t rwlock;
    pthread_cond_t funds_available;
    int transaction_count;
} Account;

typedef enum {
    DEPOSIT,
    WITHDRAW,
    TRANSFER,
    BALANCE
} TransactionType;

typedef struct {
    int id;
    TransactionType type;
    int from_account;
    int to_account;
    double amount;
    int status;
    time_t timestamp;
} Transaction;

typedef struct {
    Transaction queue[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    sem_t slots;
} TransactionQueue;
Account accounts[MAX_ACCOUNTS];
TransactionQueue tx_queue;
#endif



